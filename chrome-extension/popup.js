const competitorInput = document.getElementById("competitorInput");
const competitorDropdown = document.getElementById("competitorDropdown");
const toggleListBtn = document.getElementById("toggleListBtn");
const startBtn = document.getElementById("startBtn");
const stopBtn = document.getElementById("stopBtn");
const exportBtn = document.getElementById("exportBtn");
const clearBtn = document.getElementById("clearBtn");
const resultsList = document.getElementById("resultsList");

let competitors = [];
let selectedCompetitor = null;
let capturedResults = [];
let dropdownOpen = false;

async function loadCompetitors() {
  const response = await fetch(chrome.runtime.getURL("config/competitors.json"));
  competitors = await response.json();
  renderDropdownOptions(competitors);
}

function updateButtons(status) {
  const hasCompetitor = Boolean(selectedCompetitor);
  startBtn.disabled = !hasCompetitor || status.isRecording;
  stopBtn.disabled = !status.isRecording;
  exportBtn.disabled = capturedResults.length === 0;
  clearBtn.disabled = capturedResults.length === 0;
}

function renderResults() {
  resultsList.innerHTML = "";
  if (capturedResults.length === 0) {
    const empty = document.createElement("li");
    empty.className = "results__empty";
    empty.textContent = "No results yet.";
    resultsList.appendChild(empty);
    exportBtn.disabled = true;
    return;
  }

  capturedResults.forEach((value) => {
    const item = document.createElement("li");
    item.textContent = value;
    resultsList.appendChild(item);
  });
  exportBtn.disabled = false;
}

function resolveCompetitor(label) {
  return competitors.find((competitor) => competitor.label === label) || null;
}

function renderDropdownOptions(list) {
  competitorDropdown.innerHTML = "";
  if (list.length === 0) {
    const empty = document.createElement("div");
    empty.className = "select__option";
    empty.textContent = "No matches";
    competitorDropdown.appendChild(empty);
    return;
  }

  list.forEach((competitor) => {
    const option = document.createElement("div");
    option.className = "select__option";
    option.textContent = competitor.label;
    option.dataset.id = competitor.id;
    option.addEventListener("click", () => {
      selectedCompetitor = competitor;
      competitorInput.value = competitor.label;
      closeDropdown();
      updateButtons({ isRecording: false });
    });
    competitorDropdown.appendChild(option);
  });
}

function openDropdown() {
  dropdownOpen = true;
  competitorDropdown.classList.remove("hidden");
}

function closeDropdown() {
  dropdownOpen = false;
  competitorDropdown.classList.add("hidden");
}

function toggleDropdown() {
  if (dropdownOpen) {
    closeDropdown();
  } else {
    openDropdown();
  }
}

async function refreshStatus() {
  const status = await chrome.runtime.sendMessage({ action: "getStatus" });
  selectedCompetitor = status.activeCompetitor || selectedCompetitor;
  capturedResults = status.results || [];

  if (status.activeCompetitor) {
    competitorInput.value = status.activeCompetitor.label;
  }
  renderDropdownOptions(competitors);

  renderResults();
  updateButtons(status);
}

competitorInput.addEventListener("input", () => {
  const query = competitorInput.value.trim().toLowerCase();
  const filtered = competitors.filter((competitor) =>
    competitor.label.toLowerCase().includes(query)
  );
  renderDropdownOptions(filtered);
  if (!dropdownOpen) {
    openDropdown();
  }
  selectedCompetitor = resolveCompetitor(competitorInput.value);
  updateButtons({ isRecording: false });
});

competitorInput.addEventListener("focus", () => {
  renderDropdownOptions(competitors);
  openDropdown();
});

toggleListBtn.addEventListener("click", (event) => {
  event.preventDefault();
  renderDropdownOptions(competitors);
  toggleDropdown();
});

document.addEventListener("click", (event) => {
  if (!event.target.closest(".select")) {
    closeDropdown();
  }
});

startBtn.addEventListener("click", async () => {
  if (!selectedCompetitor) {
    return;
  }
  await chrome.runtime.sendMessage({
    action: "startRecording",
    competitorId: selectedCompetitor.id,
  });
  await refreshStatus();
});

stopBtn.addEventListener("click", async () => {
  await chrome.runtime.sendMessage({ action: "stopRecording" });
  await refreshStatus();
});

exportBtn.addEventListener("click", async () => {
  if (capturedResults.length === 0) {
    return;
  }

  const header = "payment_method";
  const rows = capturedResults.map((value) => `"${value.replace(/"/g, '""')}"`);
  const csv = [header, ...rows].join("\n");
  const blobUrl = URL.createObjectURL(new Blob([csv], { type: "text/csv" }));
  await chrome.downloads.download({
    url: blobUrl,
    filename: "payment-methods.csv",
    saveAs: true,
  });
  setTimeout(() => URL.revokeObjectURL(blobUrl), 5000);
});

clearBtn.addEventListener("click", async () => {
  await chrome.runtime.sendMessage({ action: "clearResults" });
  await refreshStatus();
});

chrome.runtime.onMessage.addListener((message) => {
  if (message.action === "resultsUpdated") {
    capturedResults = message.results;
    renderResults();
    updateButtons({ isRecording: message.isRecording });
  }
  if (message.action === "recordingStatus") {
    updateButtons({ isRecording: message.isRecording });
  }
});

(async () => {
  await loadCompetitors();
  await refreshStatus();
})();
