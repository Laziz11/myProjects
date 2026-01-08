import React, { useEffect, useMemo, useRef, useState } from "react";
import { motion, AnimatePresence } from "framer-motion";
import { createInitialRules } from "../data/rulesSeed.js";
import { setRulesState } from "../state/rulesRegistry.js";

function Switch({ checked, onChange, disabled = false, label }) {
  const handleKey = (e) => {
    if (e.key === "Enter" || e.key === " ") {
      e.preventDefault();
      if (!disabled) onChange(!checked);
    }
  };
  return (
    <button
      type="button"
      role="switch"
      aria-checked={checked}
      aria-label={label || "toggle"}
      onKeyDown={handleKey}
      onClick={() => !disabled && onChange(!checked)}
      className={`relative inline-flex h-6 w-11 items-center rounded-full transition ${
        checked ? "bg-green-500" : "bg-gray-400"
      } ${disabled ? "opacity-50 cursor-not-allowed" : "cursor-pointer"}`}
    >
      <span
        className={`absolute left-0.5 top-0.5 h-5 w-5 rounded-full bg-white transition-transform ${
          checked ? "translate-x-5" : ""
        }`}
      />
    </button>
  );
}

function Combobox({ value, onChange, options, placeholder = "Select...", className = "" }) {
  const [open, setOpen] = useState(false);
  const [query, setQuery] = useState("");
  const ref = useRef(null);
  const filtered = useMemo(
    () =>
      options.filter(
        (o) =>
          (o.label || "").toLowerCase().includes(query.toLowerCase()) ||
          (o.value || "").toLowerCase().includes(query.toLowerCase())
      ),
    [options, query]
  );
  useEffect(() => {
    const onDoc = (e) => {
      if (ref.current && !ref.current.contains(e.target)) setOpen(false);
    };
    document.addEventListener("mousedown", onDoc);
    return () => document.removeEventListener("mousedown", onDoc);
  }, []);
  const selected = options.find((o) => o.value === value);
  return (
    <div className={`relative ${className}`} ref={ref}>
      <button
        type="button"
        className="w-full rounded-lg border border-gray-300 px-2 py-2 text-left text-sm bg-white"
        onClick={() => setOpen((v) => !v)}
      >
        {selected && selected.label ? selected.label : (
          <span className="text-gray-400">{placeholder}</span>
        )}
      </button>
      <AnimatePresence>
        {open && (
          <motion.div
            initial={{ opacity: 0, y: -6 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: -6 }}
            className="absolute z-20 mt-1 w-full rounded-lg border border-gray-300 bg-white p-2 shadow-lg"
          >
            <input
              autoFocus
              value={query}
              onChange={(e) => setQuery(e.target.value)}
              placeholder="Search..."
              className="mb-2 w-full rounded-md border border-gray-300 px-2 py-1.5 text-sm outline-none"
            />
            <div className="max-h-48 overflow-y-auto">
              {filtered.length === 0 && (
                <div className="px-2 py-1.5 text-xs text-gray-500">No results</div>
              )}
              {filtered.map((opt) => (
                <button
                  key={opt.value}
                  onClick={() => {
                    onChange(opt.value);
                    setOpen(false);
                    setQuery("");
                  }}
                  className={`block w-full rounded-md px-2 py-1.5 text-left text-sm hover:bg-gray-100 ${
                    opt.value === value ? "bg-gray-100" : ""
                  }`}
                >
                  {opt.label || opt.value}
                </button>
              ))}
            </div>
          </motion.div>
        )}
      </AnimatePresence>
    </div>
  );
}

const RESULT_TYPES = ["Score", "Mark"];

const SYSTEM_LISTS = [
  { value: "blocked_emails", label: "Blocked Emails", size: 320, available: true },
  { value: "vip_allowlist", label: "VIP Allowlist", size: 120, available: true },
  { value: "blocked_ips", label: "Blocked IPs", size: 980, available: true },
  { value: "legacy_bounces", label: "Legacy Bounce DB", size: 1205, available: true },
  { value: "offline_shadow", label: "Offline Shadow List", size: 240, available: false },
];

const MAX_SYSTEM_LIST_SIZE = 1000;
const MAX_SYSTEM_LISTS_PER_RULE = 3;
const MAX_GROUP_DEPTH = 4;

const PARAMS = [
  { key: "label", label: "Label", valueType: "StringUniversal" },
  { key: "tenant", label: "Tenant", valueType: "StringEnum", options: ["4Rabet", "Crorebet", "Crorewin"] },
  { key: "first_deposit_date", label: "FD (First Deposit Date)", valueType: "Date" },
  { key: "registration_date", label: "Registration Date", valueType: "Date" },
  { key: "deposits_count", label: "Deposits Count", valueType: "Int" },
  { key: "payouts_count", label: "Payouts Count", valueType: "Int" },
  { key: "deposits_sum", label: "Deposits Sum", valueType: "Int" },
  { key: "payouts_sum", label: "Payouts Sum", valueType: "Int" },
  { key: "multi_account", label: "Multi-account", valueType: "Int" },
  { key: "platform", label: "Platform", valueType: "StringEnum", options: ["PC", "IOS", "Android"] },
  { key: "risk_mark", label: "Risk Mark", valueType: "Boolean" },
  { key: "fingerprint", label: "Fingerprint", valueType: "StringUniversal" },
  { key: "email", label: "Email", valueType: "StringUniversal" },
  { key: "telephone_number", label: "Telephone Number", valueType: "StringUniversal" },
  { key: "mark_fake", label: "Mark Fake", valueType: "Boolean" },
  { key: "ip", label: "IP", valueType: "StringUniversal" },
  { key: "first_name", label: "First Name", valueType: "StringUniversal" },
  { key: "last_name", label: "Last Name", valueType: "StringUniversal" },
];

const OPERATORS = {
  Int: [">", "<", "==", "!=", "≥", "≤"],
  Date: [">", "<", "==", "!=", "≥", "≤"],
  StringEnum: ["==", "!="],
  StringUniversal: ["==", "!=", "has", "not has"],
  Boolean: ["==", "!="],
};

const STRING_UNIVERSAL_MODES = {
  MANUAL: "manual",
  LIST: "list",
  SUBSTRING: "substring",
};

const DEFAULT_STRING_UNIVERSAL_VALUE = {
  mode: STRING_UNIVERSAL_MODES.MANUAL,
  previousMode: STRING_UNIVERSAL_MODES.MANUAL,
  text: "",
  list: "",
  substring: "",
};

const getSystemListMeta = (value) => SYSTEM_LISTS.find((item) => item.value === value) || null;

const uid = () => Math.random().toString(36).slice(2, 9);
const now = () => new Date().toISOString();
const MAX_SUBGROUPS_PER_GROUP = 4;
const createGroup = (level = 1) => ({
  id: uid(),
  level,
  operator: "AND",
  not: false,
  conditions: [],
  subgroups: [],
});

const applyGroupNames = (groups) => {
  const counters = { 1: 0, 2: 0, 3: 0, 4: 0 };
  const walk = (list) =>
    (list || []).map((g) => {
      const level = g.level || 1;
      counters[level] = (counters[level] || 0) + 1;
      const prefix = level === 1 ? "Group " : "Subgroup ";
      const name = `${prefix}${counters[level]} (level ${level})`;
      return { ...g, level, name, subgroups: walk(g.subgroups || []) };
    });
  return walk(groups || []);
};

function useDebounced(value, ms = 250) {
  const [v, setV] = useState(value);
  useEffect(() => {
    const t = setTimeout(() => setV(value), ms);
    return () => clearTimeout(t);
  }, [value, ms]);
  return v;
}

function sanitizeStringInput(str) {
  if (typeof str !== "string") return "";
  return str.trim();
}

function normalizeStringUniversal(v, operator) {
  if (!v) {
    const base = { ...DEFAULT_STRING_UNIVERSAL_VALUE };
    if (operator === "has" || operator === "not has") {
      base.mode = STRING_UNIVERSAL_MODES.SUBSTRING;
    }
    return base;
  }
  if (typeof v === "string") {
    const mode = operator === "has" || operator === "not has" ? STRING_UNIVERSAL_MODES.SUBSTRING : STRING_UNIVERSAL_MODES.MANUAL;
    return {
      ...DEFAULT_STRING_UNIVERSAL_VALUE,
      mode,
      previousMode: STRING_UNIVERSAL_MODES.MANUAL,
      text: v,
      substring: v,
    };
  }
  const storedMode = typeof v.mode === "string" ? v.mode : STRING_UNIVERSAL_MODES.MANUAL;
  const storedPrevious = typeof v.previousMode === "string" ? v.previousMode : STRING_UNIVERSAL_MODES.MANUAL;
  const modeFromOperator =
    operator === "has" || operator === "not has"
      ? STRING_UNIVERSAL_MODES.SUBSTRING
      : storedMode === STRING_UNIVERSAL_MODES.LIST
      ? STRING_UNIVERSAL_MODES.LIST
      : STRING_UNIVERSAL_MODES.MANUAL;
  return {
    mode: modeFromOperator,
    previousMode:
      storedMode === STRING_UNIVERSAL_MODES.SUBSTRING ? storedPrevious : storedMode,
    text: typeof v.text === "string" ? v.text : "",
    list: typeof v.list === "string" ? v.list : "",
    substring:
      typeof v.substring === "string"
        ? v.substring
        : typeof v.text === "string"
        ? v.text
        : "",
  };
}

function isStringUniversalValid(v, operator) {
  const { mode, text, list, substring } = normalizeStringUniversal(v, operator);
  if (mode === STRING_UNIVERSAL_MODES.SUBSTRING) return sanitizeStringInput(substring).length > 0;
  if (mode === STRING_UNIVERSAL_MODES.LIST) return Boolean(list);
  return sanitizeStringInput(text).length > 0;
}

function prepareStringUniversalForOperator(v, operator) {
  const normalized = normalizeStringUniversal(v, operator);
  if (operator === "has" || operator === "not has") {
    return {
      ...normalized,
      mode: STRING_UNIVERSAL_MODES.SUBSTRING,
      previousMode:
        normalized.mode === STRING_UNIVERSAL_MODES.SUBSTRING
          ? normalized.previousMode || STRING_UNIVERSAL_MODES.MANUAL
          : normalized.mode,
      substring: normalized.substring,
    };
  }
  if (operator === "==" || operator === "!=") {
    const fallbackMode =
      normalized.mode === STRING_UNIVERSAL_MODES.SUBSTRING
        ? normalized.previousMode || STRING_UNIVERSAL_MODES.MANUAL
        : normalized.previousMode || normalized.mode || STRING_UNIVERSAL_MODES.MANUAL;
    const nextMode =
      fallbackMode === STRING_UNIVERSAL_MODES.LIST ? STRING_UNIVERSAL_MODES.LIST : STRING_UNIVERSAL_MODES.MANUAL;
    return { ...normalized, mode: nextMode, previousMode: nextMode };
  }
  if (!operator) {
    return { ...DEFAULT_STRING_UNIVERSAL_VALUE };
  }
  return normalized;
}

function Modal({ open, onClose, title, children, maxW = "max-w-4xl" }) {
  useEffect(() => {
    if (!open) return;
    const onKey = (e) => e.key === "Escape" && onClose();
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [open, onClose]);

  return (
    <AnimatePresence>
      {open && (
        <motion.div
          className="fixed inset-0 z-50"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          exit={{ opacity: 0 }}
        >
          <div className="absolute inset-0 bg-black/50 backdrop-blur-sm" onClick={onClose} />
          <motion.div
            className={
              "absolute inset-x-0 top-6 mx-auto w-[calc(100%-2rem)] " +
              maxW +
              " rounded-2xl bg-white p-8 text-slate-900 shadow-2xl ring-1 ring-black/5"
            }
            initial={{ y: 30, scale: 0.97, opacity: 0 }}
            animate={{ y: 0, scale: 1, opacity: 1 }}
            exit={{ y: 30, scale: 0.97, opacity: 0 }}
          >
            <div className="flex items-start justify-between gap-6 sticky top-0 bg-inherit pb-4">
              <h2 className="text-2xl font-semibold">{title}</h2>
              <button
                onClick={onClose}
                className="h-9 w-9 rounded-full border border-gray-300 shadow-sm"
              >
                ×
              </button>
            </div>

            <div className="mt-4 max-h-[80vh] overflow-y-auto pr-3">{children}</div>
          </motion.div>
        </motion.div>
      )}
    </AnimatePresence>
  );
}

function Confirm({ open, onClose, onConfirm, text }) {
  return (
    <Modal open={open} onClose={onClose} title="Confirm">
      <p className="text-sm text-gray-800">{text}</p>
      <div className="mt-6 flex justify-end gap-3">
        <button className="h-10 rounded-xl border border-gray-300 px-4 text-sm" onClick={onClose}>
          Cancel
        </button>
        <button
          className="h-10 rounded-xl bg-black px-4 text-sm font-semibold text-white"
          onClick={() => {
            onConfirm();
            onClose();
          }}
        >
          Confirm
        </button>
      </div>
    </Modal>
  );
}

function GroupBlock({
  group,
  onAddCondition,
  onAddSubgroup,
  onDeleteGroup,
  onDeleteCondition,
  onDuplicateCondition,
  onUpdateCondition,
  onUpdateGroup,
  showConnector = false,
}) {
  const level = group.level || 1;
  const baseLabel = group.name || (level === 1 ? "Group (Level 1)" : `Subgroup (Level ${level})`);
  const canAddSubgroup = level < MAX_GROUP_DEPTH;

  return (
    <div className="relative pl-4">
      {showConnector && <span className="absolute left-1 top-0 bottom-0 w-px bg-slate-200" aria-hidden />}
      <div className="flex flex-wrap items-start gap-3 rounded-md border border-slate-200 bg-white px-3 py-2 shadow-sm">
        <div className="flex flex-wrap items-center gap-2">
          <label className="inline-flex items-center gap-2 text-xs text-slate-600">
            <input
              type="checkbox"
              checked={!!group.not}
              onChange={(e) => onUpdateGroup(group.id, { not: e.target.checked })}
            />
            NOT
          </label>
          <span className="inline-flex items-center rounded bg-slate-100 px-2 py-1 text-xs font-semibold text-slate-700">
            {baseLabel}
          </span>
          <div className="inline-flex items-center gap-2 text-xs text-slate-600">
            <span className="uppercase tracking-wide text-slate-500">Operator</span>
            <select
              className="rounded border border-slate-300 px-2 py-1 text-xs"
              value={group.operator || "AND"}
              onChange={(e) => onUpdateGroup(group.id, { operator: e.target.value })}
            >
              <option value="AND">AND</option>
              <option value="OR">OR</option>
            </select>
            <span className="text-slate-500">applies to items in this branch</span>
          </div>
        </div>

        <div className="ml-auto flex flex-wrap items-center gap-2 text-xs">
          <button className="rounded border border-slate-300 px-3 py-1.5" onClick={() => onAddCondition(group.id)}>
            Add condition
          </button>
          <button
            className="rounded border border-slate-300 px-3 py-1.5 disabled:opacity-50"
            disabled={!canAddSubgroup}
            onClick={() => onAddSubgroup(group.id)}
          >
            Add subgroup
          </button>
          <button
            className="rounded border border-slate-300 px-3 py-1.5 text-red-600"
            onClick={() => onDeleteGroup(group.id, group.level)}
          >
            Delete group
          </button>
        </div>
      </div>

      <div className="mt-2 space-y-2 border-l border-dashed border-slate-200 pl-4">
        {(group.conditions?.length || 0) === 0 && (group.subgroups?.length || 0) === 0 && (
          <p className="rounded border border-dashed border-slate-200 bg-white px-3 py-2 text-xs text-slate-600">
            Each group must contain at least one condition before saving.
          </p>
        )}

        {(group.conditions || []).map((c) => {
          const p = PARAMS.find((x) => x.key === c.parameter);
          const suVal =
            p?.valueType === "StringUniversal" ? normalizeStringUniversal(c.value, c.operator) : c.value;
          const listMeta = p?.valueType === "StringUniversal" && suVal?.list ? getSystemListMeta(suVal.list) : null;
          const listUnavailable =
            p?.valueType === "StringUniversal" && suVal?.mode === STRING_UNIVERSAL_MODES.LIST && suVal.list && (!listMeta || listMeta.available === false);
          const listTooLarge =
            p?.valueType === "StringUniversal" && suVal?.mode === STRING_UNIVERSAL_MODES.LIST && suVal.list && listMeta && listMeta.size > MAX_SYSTEM_LIST_SIZE;

          return (
            <div key={c.id} className="flex flex-wrap items-center gap-2 rounded-md border border-slate-200 bg-white px-3 py-2 shadow-sm">
              <label className="inline-flex items-center gap-2 text-xs text-slate-600">
                <input type="checkbox" checked={!!c.not} onChange={(e) => onUpdateCondition(group.id, c.id, { not: e.target.checked })} />
                NOT
              </label>

              <Combobox
                className="min-w-[240px]"
                value={c.parameter || ""}
                onChange={(v) => {
                  const nextParam = PARAMS.find((pp) => pp.key === v);
                  const nextValue = nextParam?.valueType === "StringUniversal" ? { ...DEFAULT_STRING_UNIVERSAL_VALUE } : "";
                  onUpdateCondition(group.id, c.id, {
                    parameter: v,
                    operator: undefined,
                    value: nextValue,
                  });
                }}
                options={PARAMS.map((pp) => ({ value: pp.key, label: pp.label }))}
                placeholder="Select parameter"
              />

              <select
                disabled={!p}
                className="rounded border border-slate-300 px-2 py-2 text-sm disabled:opacity-50"
                value={c.operator || ""}
                onChange={(e) => {
                  const nextOp = e.target.value;
                  const nextValue = p?.valueType === "StringUniversal" ? prepareStringUniversalForOperator(c.value, nextOp) : c.value;
                  onUpdateCondition(group.id, c.id, { operator: nextOp, value: nextValue });
                }}
              >
                <option value="">Operator</option>
                {p &&
                  OPERATORS[p.valueType].map((op) => (
                    <option key={op} value={op}>
                      {op}
                    </option>
                  ))}
              </select>

              {p?.valueType === "Int" && (
                <input
                  type="number"
                  placeholder="Enter number"
                  className="w-36 rounded border border-slate-300 px-2 py-2 text-sm"
                  value={typeof c.value === "number" ? c.value : c.value ? Number(c.value) : ""}
                  onChange={(e) =>
                    onUpdateCondition(group.id, c.id, {
                      value: e.target.value === "" ? "" : Number(e.target.value),
                    })
                  }
                />
              )}

              {p?.valueType === "StringEnum" && (
                <Combobox
                  className="w-48"
                  value={c.value || ""}
                  onChange={(v) => onUpdateCondition(group.id, c.id, { value: v })}
                  options={(p.options || []).map((opt) => ({ value: opt, label: opt }))}
                  placeholder="Select value"
                />
              )}

              {p?.valueType === "StringUniversal" && (
                <div className="flex flex-col gap-2">
                  {(c.operator === "==" || c.operator === "!=") && (
                    <div className="inline-flex rounded border border-slate-300 bg-white p-1 text-xs font-medium">
                      <button
                        type="button"
                        className={`rounded px-3 py-1 transition ${
                          suVal.mode === STRING_UNIVERSAL_MODES.MANUAL ? "bg-slate-900 text-white" : "text-slate-600 hover:bg-slate-100"
                        }`}
                        onClick={() =>
                          onUpdateCondition(group.id, c.id, {
                            value: {
                              ...suVal,
                              mode: STRING_UNIVERSAL_MODES.MANUAL,
                              previousMode: STRING_UNIVERSAL_MODES.MANUAL,
                            },
                          })
                        }
                      >
                        Manual input
                      </button>
                      <button
                        type="button"
                        className={`rounded px-3 py-1 transition ${
                          suVal.mode === STRING_UNIVERSAL_MODES.LIST ? "bg-slate-900 text-white" : "text-slate-600 hover:bg-slate-100"
                        }`}
                        onClick={() =>
                          onUpdateCondition(group.id, c.id, {
                            value: {
                              ...suVal,
                              mode: STRING_UNIVERSAL_MODES.LIST,
                              previousMode: STRING_UNIVERSAL_MODES.LIST,
                            },
                          })
                        }
                      >
                        System list
                      </button>
                    </div>
                  )}

                  {(c.operator === "==" || c.operator === "!=") && suVal.mode === STRING_UNIVERSAL_MODES.MANUAL && (
                    <input
                      type="text"
                      placeholder="Enter your text"
                      className="w-56 rounded border border-slate-300 px-2 py-2 text-sm"
                      value={suVal.text}
                      onChange={(e) =>
                        onUpdateCondition(group.id, c.id, {
                          value: { ...suVal, text: e.target.value },
                        })
                      }
                    />
                  )}

                  {(c.operator === "==" || c.operator === "!=") && suVal.mode === STRING_UNIVERSAL_MODES.LIST && (
                    <div className="space-y-1">
                      <Combobox
                        className="w-56"
                        value={suVal.list}
                        onChange={(v) =>
                          onUpdateCondition(group.id, c.id, {
                            value: { ...suVal, list: v },
                          })
                        }
                        options={SYSTEM_LISTS}
                        placeholder="Select list"
                      />
                      {listUnavailable && (
                        <div className="text-xs text-amber-700">Selected system list is unavailable.</div>
                      )}
                      {listTooLarge && (
                        <div className="text-xs text-amber-700">System list exceeds maximum allowed size (1000 items).</div>
                      )}
                    </div>
                  )}

                  {(c.operator === "has" || c.operator === "not has") && (
                    <input
                      type="text"
                      placeholder="Enter substring to match"
                      className="w-56 rounded border border-slate-300 px-2 py-2 text-sm"
                      value={suVal.substring}
                      onChange={(e) =>
                        onUpdateCondition(group.id, c.id, {
                          value: { ...suVal, substring: e.target.value },
                        })
                      }
                    />
                  )}

                  {!c.operator && <span className="text-xs text-slate-500">Select operator to configure value.</span>}
                </div>
              )}

              {p?.valueType === "Boolean" && (
                <Combobox
                  className="w-40"
                  value={typeof c.value === "boolean" ? String(c.value) : ""}
                  onChange={(v) => onUpdateCondition(group.id, c.id, { value: v === "true" })}
                  options={[
                    { value: "true", label: "True" },
                    { value: "false", label: "False" },
                  ]}
                  placeholder="True / False"
                />
              )}

              {p?.valueType === "Date" && (
                <input
                  type="date"
                  placeholder="Select date"
                  className="w-44 rounded border border-slate-300 px-2 py-2 text-sm"
                  value={c.value || ""}
                  onChange={(e) => onUpdateCondition(group.id, c.id, { value: e.target.value })}
                />
              )}

              <div className="ml-auto flex items-center gap-2">
                <button className="rounded border border-slate-300 px-2 py-1.5 text-xs" onClick={() => onDuplicateCondition(group.id, c.id)}>
                  Duplicate
                </button>
                <button className="rounded border border-slate-300 px-2 py-1.5 text-xs text-red-600" onClick={() => onDeleteCondition(group.id, c.id)}>
                  Delete
                </button>
              </div>
            </div>
          );
        })}

        {(group.subgroups || []).map((sub) => (
          <GroupBlock
            key={sub.id}
            group={sub}
            onAddCondition={onAddCondition}
            onAddSubgroup={onAddSubgroup}
            onDeleteGroup={onDeleteGroup}
            onDeleteCondition={onDeleteCondition}
            onDuplicateCondition={onDuplicateCondition}
            onUpdateCondition={onUpdateCondition}
            onUpdateGroup={onUpdateGroup}
            showConnector
          />
        ))}
      </div>
    </div>
  );
}

const normalizeRule = (rule) => ({
  ...rule,
  groupsOperator: rule.groupsOperator || "AND",
  groups: applyGroupNames(rule.groups || []),
});

const normalizeRuleList = (items) => (items || []).map((rule) => normalizeRule(rule));

function RuleForm({ initial, onSubmit, onCancel }) {
  const [rule, setRule] = useState(() => normalizeRule(initial));
  const [errors, setErrors] = useState([]);
  const [askDeleteGroup, setAskDeleteGroup] = useState(null);
  const [inlineAlert, setInlineAlert] = useState("");

  useEffect(() => {
    setRule(normalizeRule(initial));
  }, [initial]);

  const setRuleGroups = (updater) => {
    setRule((current) => {
      const nextGroups = typeof updater === "function" ? updater(current.groups || []) : updater;
      return { ...current, groups: applyGroupNames(nextGroups) };
    });
  };

    useEffect(() => {
      if (!inlineAlert) return;
      const timer = setTimeout(() => setInlineAlert(""), 2200);
      return () => clearTimeout(timer);
  }, [inlineAlert]);

  const updateGroupsById = (groups, id, updater) =>
    groups.map((group) => {
      if (group.id === id) {
        return updater(group);
      }
      return { ...group, subgroups: updateGroupsById(group.subgroups || [], id, updater) };
    });

  const findGroupById = (groups, id) => {
    for (const group of groups) {
      if (group.id === id) return group;
      const nested = findGroupById(group.subgroups || [], id);
      if (nested) return nested;
    }
    return null;
  };

  const removeGroupById = (groups, id) => {
    let removed = false;
    const next = groups
      .map((g) => {
        const nested = removeGroupById(g.subgroups || [], id);
        if (nested.removed) {
          removed = true;
        }
        return { ...g, subgroups: nested.groups };
      })
      .filter((g) => {
        if (g.id === id) {
          removed = true;
          return false;
        }
        return true;
      });
    return { groups: next, removed };
  };

  const headerErrors = useMemo(() => {
    const e = [];
    if (!rule.name || rule.name.trim().length === 0) e.push("Field required (Name)");
    if (rule.name && rule.name.length > 100) e.push("Rule name must be ≤ 100 chars");
    if (rule.description && rule.description.length > 300) e.push("Description must be ≤ 300 chars");
    return e;
  }, [rule.name, rule.description]);

  const resultErrors = useMemo(() => {
    const e = [];
    if (rule.resultType === "Score") {
      if (rule.value === null || rule.value === undefined || rule.value === "") e.push("Result value required.");
      const n = Number(rule.value);
      if (Number.isNaN(n)) e.push("Score must be a number");
      if (!Number.isNaN(n) && (n < -1000 || n > 1000)) e.push("Score must be between −1000 and 1000.");
    } else {
      if (!rule.value) e.push("Please select a Mark value");
    }
    return e;
  }, [rule.resultType, rule.value]);

  const walkGroups = (groups, cb) => {
    groups.forEach((g) => {
      cb(g);
      walkGroups(g.subgroups || [], cb);
    });
  };

  const systemListState = useMemo(() => {
    const used = new Set();
    const listErrors = new Set();

    walkGroups(rule.groups, (g) => {
      g.conditions.forEach((c) => {
        const p = PARAMS.find((x) => x.key === c.parameter);
        if (p?.valueType !== "StringUniversal") return;
        if (c.operator !== "==" && c.operator !== "!=") return;
        const suVal = normalizeStringUniversal(c.value, c.operator);
        if (suVal.mode !== STRING_UNIVERSAL_MODES.LIST || !suVal.list) return;
        used.add(suVal.list);
        const meta = getSystemListMeta(suVal.list);
        if (!meta || meta.available === false) {
          listErrors.add("Selected system list is unavailable.");
        }
        if (meta && meta.size > MAX_SYSTEM_LIST_SIZE) {
          listErrors.add("System list exceeds maximum allowed size (1000 items).");
        }
      });
    });

    return { used: Array.from(used), errors: Array.from(listErrors) };
  }, [rule.groups]);

  const builderErrors = useMemo(() => {
    const e = new Set();
    if (rule.groups.length === 0) e.add("Please add at least one group before saving.");
    if (rule.groups.length > 1 && !rule.groupsOperator) {
      e.add("Select AND/OR between level 1 groups.");
    }

    const validateGroup = (group) => {
      if ((group.level || 1) > MAX_GROUP_DEPTH) {
        e.add("Maximum nesting level (4) exceeded.");
      }
      if ((group.subgroups?.length || 0) > MAX_SUBGROUPS_PER_GROUP) {
        e.add("You can add up to 4 subgroups within a group.");
      }
      if ((group.conditions?.length || 0) === 0 && (group.subgroups?.length || 0) === 0) {
        e.add("Each group must contain at least one condition before saving.");
      }
      (group.conditions || []).forEach((c) => {
        const p = PARAMS.find((x) => x.key === c.parameter);
        if (!c.parameter || !c.operator) {
          e.add("Complete the condition or remove it.");
          return;
        }
        if (p?.valueType === "StringUniversal") {
          if (!isStringUniversalValid(c.value, c.operator)) e.add("Complete the condition or remove it.");
          return;
        }
        if (c.value === undefined || c.value === null || c.value === "") {
          e.add("Complete the condition or remove it.");
          return;
        }
        if (p && p.valueType === "Int") {
          if (typeof c.value !== "number") e.add("Value must be a number.");
        }
        if (p && p.valueType === "StringEnum" && typeof c.value !== "string") e.add("Please provide a string value.");
        if (p && p.valueType === "Boolean" && typeof c.value !== "boolean") e.add("Please select True or False.");
        if (p && p.valueType === "Date" && typeof c.value !== "string") e.add("Invalid date format.");
      });
      (group.subgroups || []).forEach(validateGroup);
    };

    rule.groups.forEach(validateGroup);

    systemListState.errors.forEach((msg) => e.add(msg));
    if (systemListState.used.length > MAX_SYSTEM_LISTS_PER_RULE) {
      e.add("You can use up to 3 system lists per rule.");
    }
    return Array.from(e);
  }, [rule.groups, systemListState]);

  const addGroup = () => {
    setRuleGroups((groups) => [...groups, createGroup(1)]);
  };

  const addSubgroup = (parentId) => {
    const parent = findGroupById(rule.groups, parentId);
    if (!parent) return;
    if ((parent.level || 1) >= MAX_GROUP_DEPTH) {
      setInlineAlert("Maximum depth of nested groups is 4.");
      return;
    }
    if ((parent.subgroups?.length || 0) >= MAX_SUBGROUPS_PER_GROUP) {
      setInlineAlert("You can add up to 4 subgroups within a group.");
      return;
    }
    setRuleGroups((groups) =>
      updateGroupsById(groups, parentId, (group) => ({
        ...group,
        subgroups: [...(group.subgroups || []), createGroup((group.level || 1) + 1)],
      }))
    );
  };

  const removeGroup = (gid) => {
    setRuleGroups((groups) => removeGroupById(groups, gid).groups);
  };

  const addCondition = (gid) => {
    setRuleGroups((groups) =>
      updateGroupsById(groups, gid, (group) => ({
        ...group,
        conditions: [...(group.conditions || []), { id: uid() }],
      }))
    );
  };

  const updateCondition = (gid, cid, patch) => {
    setRuleGroups((groups) =>
      updateGroupsById(groups, gid, (group) => ({
        ...group,
        conditions: (group.conditions || []).map((c) => (c.id === cid ? { ...c, ...patch } : c)),
      }))
    );
  };

  const deleteCondition = (gid, cid) => {
    setRuleGroups((groups) =>
      updateGroupsById(groups, gid, (group) => ({
        ...group,
        conditions: (group.conditions || []).filter((c) => c.id !== cid),
      }))
    );
  };

  const duplicateCondition = (gid, cid) => {
    setRuleGroups((groups) =>
      updateGroupsById(groups, gid, (group) => {
        const idx = (group.conditions || []).findIndex((c) => c.id === cid);
        if (idx === -1) return group;
        const source = group.conditions[idx];
        const clonedValue =
          source && typeof source.value === "object" && source.value !== null
            ? JSON.parse(JSON.stringify(source.value))
            : source.value;
        const cloned = { ...source, id: uid(), value: clonedValue };
        const arr = [...group.conditions];
        arr.splice(idx + 1, 0, cloned);
        return { ...group, conditions: arr };
      })
    );
  };

  const onSave = () => {
    const all = [...headerErrors, ...resultErrors, ...builderErrors];
    setErrors(all);
    if (all.length === 0) onSubmit({ ...rule, updatedAt: now() });
  };

  return (
    <div className="space-y-6">
      <section className="rounded-xl border border-gray-300 p-4">
        <h3 className="font-medium">Header</h3>
        <div className="mt-3 grid gap-3 sm:grid-cols-2">
          <div>
            <label className="text-sm">Name *</label>
            <input
              className="mt-1 w-full rounded-lg border border-gray-300 px-3 py-2 text-sm"
              placeholder="Rule name"
              value={rule.name}
              onChange={(e) => setRule({ ...rule, name: e.target.value })}
            />
          </div>
          <div>
            <label className="text-sm">Description</label>
            <input
              className="mt-1 w-full rounded-lg border border-gray-300 px-3 py-2 text-sm"
              placeholder="e.g., from 1 month to 4 months"
              value={rule.description || ""}
              onChange={(e) => setRule({ ...rule, description: e.target.value })}
            />
          </div>
        </div>
        <div className="mt-3 inline-flex items-center gap-2 text-sm">
          <Switch checked={rule.active} onChange={(v) => setRule({ ...rule, active: v })} label="Active" />
          <span>Active</span>
        </div>
      </section>

      <section className="rounded-xl border border-gray-300 p-4">
        <div className="flex items-center justify-between">
          <h3 className="font-medium">Rule Constructor</h3>
          <div className="flex items-center gap-2">
            <button className="rounded-lg border border-gray-300 px-3 py-2 text-sm" onClick={addGroup}>
              Add group
            </button>
          </div>
        </div>

        {(inlineAlert || systemListState.used.length > MAX_SYSTEM_LISTS_PER_RULE || (rule.groups.length > 1 && !rule.groupsOperator)) && (
          <div className="mt-3 space-y-2">
            {inlineAlert && (
              <div className="rounded-lg border border-amber-200 bg-amber-50 px-3 py-2 text-sm text-amber-800">
                {inlineAlert}
              </div>
            )}
            {systemListState.used.length > MAX_SYSTEM_LISTS_PER_RULE && (
              <div className="rounded-lg border border-amber-200 bg-amber-50 px-3 py-2 text-sm text-amber-800">
                You can use up to 3 system lists per rule.
              </div>
            )}
            {rule.groups.length > 1 && !rule.groupsOperator && (
              <div className="rounded-lg border border-amber-200 bg-amber-50 px-3 py-2 text-sm text-amber-800">
                Choose AND or OR to join all level 1 groups.
              </div>
            )}
          </div>
        )}

        {rule.groups.length === 0 ? (
          <p className="mt-4 rounded-lg border border-dashed border-gray-300 p-4 text-sm text-gray-600">
            No groups created yet. Click ‘Add group’ to start building your rule.
          </p>
        ) : (
          <div className="mt-4 space-y-4">
            {rule.groups.length > 1 && (
              <div className="flex items-center gap-2 rounded-lg border border-dashed border-gray-300 bg-white px-3 py-2 text-sm">
                <span className="text-xs uppercase tracking-wide text-gray-500">Level 1 groups join</span>
                <select
                  className="rounded-lg border border-gray-300 px-2 py-1.5 text-xs"
                  value={rule.groupsOperator || ""}
                  onChange={(e) => setRule((r) => ({ ...r, groupsOperator: e.target.value || "AND" }))}
                >
                  <option value="">Select</option>
                  <option value="AND">AND</option>
                  <option value="OR">OR</option>
                </select>
              </div>
            )}

            {rule.groups.map((g) => (
              <GroupBlock
                key={g.id}
                group={g}
                onAddCondition={addCondition}
                onAddSubgroup={addSubgroup}
                onDeleteGroup={(id, level = 1) => {
                  if ((level || 1) > 1) {
                    removeGroup(id);
                    return;
                  }
                  setAskDeleteGroup(id);
                }}
                onDeleteCondition={deleteCondition}
                onDuplicateCondition={duplicateCondition}
                onUpdateCondition={updateCondition}
                onUpdateGroup={(id, patch) =>
                  setRuleGroups((groups) => updateGroupsById(groups, id, (group) => ({ ...group, ...patch })))
                }
              />
            ))}
          </div>
        )}
      </section>

      <section className="rounded-xl border border-gray-300 p-4">
        <h3 className="font-medium">Result</h3>
        <div className="mt-3 grid gap-3 sm:grid-cols-2">
          <div>
            <label className="text-sm">Result Type</label>
            <select
              className="mt-1 w-full rounded-lg border border-gray-300 px-3 py-2 text-sm"
              value={rule.resultType}
              onChange={(e) =>
                setRule({
                  ...rule,
                  resultType: e.target.value,
                  value: e.target.value === "Score" ? 0 : "",
                })
              }
            >
              {RESULT_TYPES.map((rt) => (
                <option key={rt} value={rt}>
                  {rt}
                </option>
              ))}
            </select>
          </div>
          <div>
            <label className="text-sm">Value</label>
            {rule.resultType === "Score" ? (
              <input
                type="number"
                placeholder="Enter score (−1000 to 1000)"
                className="mt-1 w-full rounded-lg border border-gray-300 px-3 py-2 text-sm"
                value={
                  rule.value === "" || rule.value === null || rule.value === undefined
                    ? ""
                    : rule.value
                }
                onChange={(e) =>
                  setRule({
                    ...rule,
                    value: e.target.value === "" ? "" : Number(e.target.value),
                  })
                }
              />
            ) : (
              <select
                className="mt-1 w-full rounded-lg border border-gray-300 px-3 py-2 text-sm"
                value={rule.value || ""}
                onChange={(e) => setRule({ ...rule, value: e.target.value })}
              >
                <option value="">Select Mark</option>
                <option value="Fraud">Fraud</option>
                <option value="VIP">VIP</option>
                <option value="Watchlist">Watchlist</option>
              </select>
            )}
          </div>
        </div>
      </section>

      {errors.length > 0 && (
        <div className="rounded-xl border border-red-200 bg-red-50 p-4 text-sm text-red-800">
          <ul className="list-disc pl-5">
            {errors.map((e, i) => (
              <li key={i}>{e}</li>
            ))}
          </ul>
        </div>
      )}

      <div className="flex justify-end gap-3">
        <button onClick={onCancel} className="h-10 rounded-xl border border-gray-300 px-4 text-sm">
          Cancel
        </button>
        <button onClick={onSave} className="h-10 rounded-xl bg-black px-4 text-sm font-semibold text-white">
          Save
        </button>
      </div>

      <Confirm
        open={!!askDeleteGroup}
        onClose={() => setAskDeleteGroup(null)}
        onConfirm={() => {
          if (askDeleteGroup) removeGroup(askDeleteGroup);
        }}
        text={`Are you sure you want to delete this group?`}
      />
    </div>
  );
}

export default function RulesPage({ focusRuleId, onRuleFocusHandled }) {
  const [rules, setRules] = useState(() => {
    const seeded = normalizeRuleList(createInitialRules());
    setRulesState(seeded);
    return seeded;
  });
  const [query, setQuery] = useState("");
  const q = useDebounced(query, 200);
  const [permission, setPermission] = useState("Delete");
  const [toast, setToast] = useState(null);
  const [editing, setEditing] = useState(null);
  const [creating, setCreating] = useState(false);
  const [askDeleteRule, setAskDeleteRule] = useState(null);

  useEffect(() => {
    setRulesState(rules);
  }, [rules]);

  useEffect(() => {
    if (!focusRuleId) return;
    const target = rules.find((rule) => rule.id === focusRuleId);
    if (target) {
      setEditing(target);
    }
    if (typeof onRuleFocusHandled === "function") {
      onRuleFocusHandled();
    }
  }, [focusRuleId, rules, onRuleFocusHandled]);

  const filtered = useMemo(() => {
    const v = q.trim().toLowerCase();
    if (!v) return rules;
    return rules.filter(
      (r) =>
        r.name.toLowerCase().includes(v) || (r.description || "").toLowerCase().includes(v)
    );
  }, [q, rules]);

  const sorted = useMemo(
    () => [...filtered].sort((a, b) => (a.createdAt < b.createdAt ? 1 : -1)),
    [filtered]
  );

  const canWrite = permission === "Write" || permission === "Delete";
  const canDelete = permission === "Delete";

  const groupHasContent = (group) => {
    if ((group.conditions?.length || 0) === 0 && (group.subgroups?.length || 0) === 0) return false;
    if ((group.subgroups || []).some((sub) => !groupHasContent(sub))) return false;
    if ((group.level || 1) > MAX_GROUP_DEPTH) return false;
    return true;
  };


  useEffect(() => {
    if (!toast) return;
    const timer = setTimeout(() => setToast(null), 2200);
    return () => clearTimeout(timer);
  }, [toast]);



  const duplicateRule = (id) => {
    const r = rules.find((x) => x.id === id);
    if (!r) return;
    const copy = {
      ...r,
      id: uid(),
      name: `Copy of ${r.name}`.slice(0, 100),
      active: false,
      createdAt: now(),
      updatedAt: now(),
    };
    setRules((rs) => [copy, ...rs]);
  };

  const toggleActive = (id) => {
    setRules((rs) =>
      rs.map((r) => {
        if (r.id !== id) return r;
        const groupsOk = r.groups.length > 0 && r.groups.every((g) => groupHasContent(g));
        const resultOk =
          r.resultType === "Score"
            ? typeof r.value === "number" && r.value >= -1000 && r.value <= 1000
            : Boolean(r.value);
        if (!groupsOk || !resultOk) {
          setToast("Cannot activate: fix builder/result errors first");
          return r;
        }
        return { ...r, active: !r.active, updatedAt: now() };
      })
    );
  };

  return (
    <div className="min-h-screen bg-gray-50 p-6 text-gray-900">
      <div className="mx-auto max-w-6xl">
        <header className="mb-6 flex items-center justify-between gap-4">
          <div>
            <h1 className="text-2xl font-bold">Scoring Rules</h1>
          </div>
          <div className="flex items-center gap-2">
            <label className="text-sm">Permission:</label>
            <select
              className="rounded-lg border border-gray-300 px-2 py-2 text-sm"
              value={permission}
              onChange={(e) => setPermission(e.target.value)}
            >
              <option>Read</option>
              <option>Write</option>
              <option>Delete</option>
            </select>
          </div>
        </header>

        <div className="mb-4 flex items-center gap-3">
          <button
            disabled={!canWrite}
            onClick={() => setCreating(true)}
            className="h-11 rounded-xl bg-black px-4 text-sm font-semibold text-white shadow-lg transition disabled:cursor-not-allowed disabled:opacity-50"
            title={canWrite ? "+ ADD" : "Insufficient permissions"}
          >
            + ADD
          </button>
          <div className="ml-auto flex items-center gap-2">
            <span className="text-sm text-gray-500">🔎</span>
            <input
              placeholder="Search by Name or Description"
              className="w-72 rounded-xl border border-gray-300 px-3 py-2 text-sm"
              value={query}
              onChange={(e) => setQuery(e.target.value)}
            />
          </div>
        </div>

        <div className="grid grid-cols-1 gap-3">
          {sorted.map((r) => (
            <div key={r.id} className="rounded-2xl border border-gray-300 bg-white p-4 shadow-sm">
              <div className="flex flex-wrap items-start gap-4">
                <div className="min-w-0 flex-1">
                  <div className="flex items-center gap-3">
                    <h3 className="truncate text-lg font-semibold">{r.name}</h3>
                    <span className="rounded-full border border-gray-300 px-2 py-0.5 text-xs">{r.resultType}</span>
                    <span className="rounded-full border border-gray-300 px-2 py-0.5 text-xs">Value: {String(r.value)}</span>
                  </div>
                  <p className="mt-1 line-clamp-2 text-sm text-gray-600 dark:text-neutral-300">{r.description}</p>
                  <div className="mt-2 text-xs text-gray-500">
                    Created At: {new Date(r.createdAt).toLocaleString()} • Updated At: {new Date(r.updatedAt).toLocaleString()}
                  </div>
                </div>
                <div className="flex flex-col items-end gap-2">
                  <div className="inline-flex items-center gap-2 text-sm">
                    <Switch
                      checked={r.active}
                      onChange={() => (canWrite ? toggleActive(r.id) : null)}
                      disabled={!canWrite}
                      label="Active"
                    />
                    <span>Active</span>
                  </div>
                  <div className="flex items-center gap-2">
                    <button className="rounded-lg border border-gray-300 px-3 py-1.5 text-sm disabled:opacity-50" disabled={!canWrite} onClick={() => setEditing(r)}>
                      Edit
                    </button>
                    <button className="rounded-lg border border-gray-300 px-3 py-1.5 text-sm disabled:opacity-50" disabled={!canWrite} onClick={() => duplicateRule(r.id)}>
                      Duplicate
                    </button>
                    <button
                      className="rounded-lg border border-gray-300 px-3 py-1.5 text-sm text-red-600 disabled:opacity-50"
                      disabled={!canDelete || r.active}
                      onClick={() => (r.active ? null : setAskDeleteRule(r.id))}
                    >
                      Delete
                    </button>
                  </div>
                </div>
              </div>
            </div>
          ))}
        </div>

        {sorted.length === 0 && (
          <div className="mt-10 rounded-2xl border border-gray-300 bg-white p-6 text-center text-sm text-gray-600">
            No rules match your search.
          </div>
        )}
      </div>

      <Modal open={creating} onClose={() => setCreating(false)} title="Create Rule" maxW="max-w-4xl">
        <RuleForm
          initial={{
            id: uid(),
            name: "",
            description: "",
            resultType: "Score",
            value: 0,
            createdAt: now(),
            updatedAt: now(),
            active: true,
            groups: [],
            groupsOperator: "AND",
          }}
          onCancel={() => setCreating(false)}
          onSubmit={(r) => {
            setRules((rs) => [
              { ...normalizeRule(r), id: uid(), createdAt: now(), updatedAt: now() },
              ...rs,
            ]);
            setCreating(false);
          }}
        />
      </Modal>

      <Modal open={!!editing} onClose={() => setEditing(null)} title="Edit Rule" maxW="max-w-4xl">
        {editing && (
          <RuleForm
            initial={editing}
            onCancel={() => setEditing(null)}
            onSubmit={(r) => {
              setRules((rs) => rs.map((x) => (x.id === r.id ? normalizeRule(r) : x)));
              setEditing(null);
            }}
          />
        )}
      </Modal>

      <Confirm
        open={!!askDeleteRule}
        onClose={() => setAskDeleteRule(null)}
        onConfirm={() => {
          if (askDeleteRule) {
            setRules((rs) => rs.filter((r) => r.id !== askDeleteRule));
          }
        }}
        text={`Are you sure you want to delete this rule?`}
      />

      {toast && (
        <div className="fixed bottom-6 right-6 rounded-xl bg-black px-4 py-2 text-sm text-white shadow-lg">
          {toast}
        </div>
      )}
    </div>
  );
}
