import { useSyncExternalStore } from "react";

let rulesState = [];
const listeners = new Set();

function emit() {
  listeners.forEach((listener) => listener());
}

export function setRulesState(nextRules) {
  rulesState = Array.isArray(nextRules) ? nextRules.map((rule) => ({ ...rule })) : [];
  emit();
}

function getSnapshot() {
  return rulesState;
}

export function subscribe(listener) {
  listeners.add(listener);
  return () => {
    listeners.delete(listener);
  };
}

export function useRulesSnapshot() {
  return useSyncExternalStore(subscribe, getSnapshot, getSnapshot);
}

export function getRuleMetadata(ruleId) {
  if (!ruleId) return null;
  const rule = rulesState.find((item) => item.id === ruleId);
  if (!rule) return null;
  return {
    id: rule.id,
    name: rule.name,
    description: rule.description,
  };
}
