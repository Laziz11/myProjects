let competitorsCache = null;
let activeCompetitor = null;
let isRecording = false;
let resultsSet = new Set();
let attachedTabId = null;
const trackedRequests = new Map();

async function loadCompetitors() {
  if (competitorsCache) {
    return competitorsCache;
  }
  const response = await fetch(chrome.runtime.getURL("config/competitors.json"));
  competitorsCache = await response.json();
  return competitorsCache;
}

function broadcastResults() {
  const results = Array.from(resultsSet).sort();
  chrome.runtime.sendMessage({
    action: "resultsUpdated",
    results,
    isRecording,
  });
  chrome.storage.session.set({
    results,
    isRecording,
    activeCompetitor,
  });
}

function extractValues(obj, path) {
  const segments = path.split(".").filter(Boolean);
  let currentValues = [obj];

  for (const segment of segments) {
    const nextValues = [];
    const isWildcard = segment === "*";
    const isKeySelector = segment === "@key";
    const isArraySegment = segment.endsWith("[]");
    const key = isArraySegment ? segment.slice(0, -2) : segment;

    for (const value of currentValues) {
      if (!value || typeof value !== "object") {
        continue;
      }

      if (isKeySelector) {
        if (!Array.isArray(value)) {
          nextValues.push(...Object.keys(value));
        }
        continue;
      }

      if (isWildcard) {
        if (Array.isArray(value)) {
          nextValues.push(...value);
        } else {
          nextValues.push(...Object.values(value));
        }
        continue;
      }

      if (!(key in value)) {
        continue;
      }

      const nextValue = value[key];
      if (isArraySegment && Array.isArray(nextValue)) {
        nextValues.push(...nextValue);
      } else {
        nextValues.push(nextValue);
      }
    }

    currentValues = nextValues;
  }

  return currentValues
    .flatMap((value) => (Array.isArray(value) ? value : [value]))
    .filter((value) => typeof value === "string" && value.trim().length > 0)
    .map((value) => value.trim());
}

function decodeBody({ body, base64Encoded }) {
  if (!base64Encoded) {
    return body;
  }
  try {
    return atob(body);
  } catch (error) {
    return "";
  }
}

async function sendDebugCommand(tabId, method, params = {}) {
  return chrome.debugger.sendCommand({ tabId }, method, params);
}

async function attachDebuggerToActiveTab() {
  const [tab] = await chrome.tabs.query({ active: true, currentWindow: true });
  if (!tab || tab.id == null) {
    return null;
  }

  await chrome.debugger.attach({ tabId: tab.id }, "1.3");
  await sendDebugCommand(tab.id, "Network.enable");
  attachedTabId = tab.id;
  return tab.id;
}

async function detachDebugger() {
  if (attachedTabId == null) {
    return;
  }
  const tabId = attachedTabId;
  attachedTabId = null;
  trackedRequests.clear();
  try {
    await chrome.debugger.detach({ tabId });
  } catch (error) {
    // Ignore detach failures.
  }
}

async function handleDebuggerEvent(source, method, params) {
  if (!isRecording || !activeCompetitor) {
    return;
  }

  if (source.tabId !== attachedTabId) {
    return;
  }

  if (method === "Network.responseReceived") {
    const responseUrl = params.response?.url || "";
    if (!responseUrl.includes(activeCompetitor.requestUrlIncludes)) {
      return;
    }
    trackedRequests.set(params.requestId, responseUrl);
    return;
  }

  if (method === "Network.loadingFailed") {
    trackedRequests.delete(params.requestId);
    return;
  }

  if (method !== "Network.loadingFinished") {
    return;
  }

  const responseUrl = trackedRequests.get(params.requestId);
  if (!responseUrl) {
    return;
  }
  trackedRequests.delete(params.requestId);

  let bodyResult;
  try {
    bodyResult = await sendDebugCommand(source.tabId, "Network.getResponseBody", {
      requestId: params.requestId,
    });
  } catch (error) {
    return;
  }

  const rawBody = decodeBody(bodyResult || { body: "", base64Encoded: false });
  let payload;
  try {
    payload = JSON.parse(rawBody);
  } catch (error) {
    return;
  }

  const values = activeCompetitor.responseJsonPaths.flatMap((path) =>
    extractValues(payload, path)
  );

  if (values.length === 0) {
    return;
  }

  values.forEach((value) => resultsSet.add(value));
  broadcastResults();
}

chrome.debugger.onEvent.addListener((source, method, params) => {
  handleDebuggerEvent(source, method, params);
});

chrome.debugger.onDetach.addListener(() => {
  isRecording = false;
  attachedTabId = null;
  trackedRequests.clear();
  broadcastResults();
});

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  if (message.action === "startRecording") {
    loadCompetitors().then(async (competitors) => {
      const competitor = competitors.find(
        (item) => item.id === message.competitorId
      );
      if (!competitor) {
        sendResponse({ ok: false, error: "Competitor not found" });
        return;
      }

      try {
        await detachDebugger();
        const tabId = await attachDebuggerToActiveTab();
        if (tabId == null) {
          sendResponse({ ok: false, error: "No active tab" });
          return;
        }
      } catch (error) {
        sendResponse({ ok: false, error: "Debugger attach failed" });
        return;
      }

      activeCompetitor = competitor;
      isRecording = true;
      resultsSet = new Set();
      broadcastResults();
      chrome.runtime.sendMessage({
        action: "recordingStatus",
        isRecording,
      });
      sendResponse({ ok: true });
    });
    return true;
  }

  if (message.action === "stopRecording") {
    isRecording = false;
    trackedRequests.clear();
    detachDebugger();
    broadcastResults();
    chrome.runtime.sendMessage({
      action: "recordingStatus",
      isRecording,
    });
    sendResponse({ ok: true });
    return true;
  }

  if (message.action === "getStatus") {
    const results = Array.from(resultsSet).sort();
    sendResponse({
      results,
      isRecording,
      activeCompetitor,
    });
    return true;
  }

  if (message.action === "clearResults") {
    resultsSet = new Set();
    broadcastResults();
    sendResponse({ ok: true });
    return true;
  }

  return false;
});

chrome.runtime.onInstalled.addListener(async () => {
  await chrome.storage.session.set({
    results: [],
    isRecording: false,
    activeCompetitor: null,
  });
});
