import React, { useMemo, useState } from "react";
import { RULE_SEEDS } from "../data/rulesSeed.js";
import { getRuleMetadata } from "../state/rulesRegistry.js";

const fmtMoney = (n) => (n == null ? "–" : new Intl.NumberFormat().format(Number(n)));
const fmtPct = (n) => (n == null ? "–" : `${Number(n).toFixed(2)}%`);
const fmtDate = (d) => (d ? new Date(d).toISOString().split("T")[0] : "–");

const TENANTS = ["4RA", "Testbet", "Crorebet", "Crorewin", "EpsilonPlay"];
const SOURCES = ["Partnet", "Offer", "Organic", "Social", "Email", "Sponsored"];
const SYSTEM_MARKS = [
  "VIP",
  "Suspicious",
  "Review",
  "Flagged",
  "Risk Flag",
  "Chargeback",
  "Fraud Check",
  "Duplicate",
  "Promo",
  "KYC Pending",
  "Whitelisted",
];

const LIST_COLUMNS = [
  "UUID",
  "Label",
  "Tenant",
  "Risk Score (Label)",
  "FD",
  "Registration Date",
  "Source of Traffic",
  "Deposits Count",
  "Payouts Count",
  "Deposits Sum",
  "Payouts Sum",
  "Success Deposits %",
  "Multi-account",
];

const LINKED_COLUMNS = [
  "Label",
  "Tenant",
  "Risk Score (Label)",
  "Marks",
  "Change By",
  "Reason",
  "Actions",
];
const HISTORY_COLUMNS = [
  "Date / Time",
  "Event",
  "Details",
  "Previous Value",
  "New Value",
];

const PERMISSION_OPTIONS = ["Read", "Write", "Delete"];
const PERMISSION_LEVEL_ORDER = { Read: 1, Write: 2, Delete: 3 };
const DEFAULT_PERMISSION_LEVEL = "Delete";

const HISTORY_EVENTS = {
  RULE_APPLIED: "RuleApplied",
  MANUAL_ADJUSTMENT: "ManualAdjustment",
  MARK_ADDED: "MarkAdded",
  MARK_REMOVED: "MarkRemoved",
  LABEL_LINKED: "LabelLinked",
  LABEL_UNLINKED: "LabelUnlinked",
  RULE_RECALCULATION: "RuleRecalculation",
};

const RULE_IDS = {
  PAYOUT_VELOCITY: "rule-payout-velocity-alert",
  DUPLICATE_LABEL: "rule-duplicate-label-detector",
  VIP_HIGH_ROLLER: "rule-vip-high-roller-track",
};

const makeHistoryEntry = ({ event, details, previousValue = null, newValue = null, when = new Date().toISOString() }) => ({
  id: cryptoRandomUUID(),
  when,
  event,
  details,
  previousValue,
  newValue,
});

const buildRuleDetail = (seedIndex = 0) => {
  if (!RULE_SEEDS.length) {
    return { type: "rule", text: "Rule reference", ruleId: null, ruleName: "Rule reference" };
  }
  const index = Math.abs(seedIndex) % RULE_SEEDS.length;
  const rule = RULE_SEEDS[index];
  return {
    type: "rule",
    text: rule.name,
    ruleId: rule.id,
    ruleName: rule.name,
  };
};

const buildRuleDetailById = (ruleId, fallbackIndex = 0) => {
  if (ruleId) {
    const meta = getRuleMetadata(ruleId);
    if (meta) {
      return { type: "rule", text: meta.name, ruleId: meta.id, ruleName: meta.name };
    }
    const seed = RULE_SEEDS.find((candidate) => candidate.id === ruleId);
    if (seed) {
      return { type: "rule", text: seed.name, ruleId: seed.id, ruleName: seed.name };
    }
  }
  return buildRuleDetail(fallbackIndex);
};

const buildTextDetail = (text) => ({
  type: "text",
  text,
});

const normalizeReason = (reason, seedIndex = 0) => {
  if (!reason) {
    const fallback = buildRuleDetail(seedIndex);
    const meta = getRuleMetadata(fallback.ruleId);
    return meta ? { ...fallback, text: meta.name, ruleName: meta.name } : fallback;
  }
  if (typeof reason === "string") {
    if (reason.toLowerCase() === "system") {
      const fallback = buildRuleDetail(seedIndex);
      const meta = getRuleMetadata(fallback.ruleId);
      return meta ? { ...fallback, text: meta.name, ruleName: meta.name } : fallback;
    }
    return buildTextDetail(reason);
  }
  if (reason && typeof reason === "object" && (reason.type || reason.text || reason.ruleName)) {
    if (reason.type === "rule" && reason.ruleId) {
      const meta = getRuleMetadata(reason.ruleId);
      if (meta) {
        return { ...reason, text: meta.name, ruleName: meta.name };
      }
    }
    return { ...reason };
  }
  return buildTextDetail(String(reason));
};

const normalizeChangeBy = (changeBy) =>
  changeBy === "UserAdminPay" || changeBy === "System" ? changeBy : "System";

function derivePermissions(level = DEFAULT_PERMISSION_LEVEL) {
  const rank = PERMISSION_LEVEL_ORDER[level] ?? PERMISSION_LEVEL_ORDER.Read;

  return {
    "user_data:list:read": rank >= PERMISSION_LEVEL_ORDER.Read,
    "user_data:list:refresh": rank >= PERMISSION_LEVEL_ORDER.Write,
    "user_data:list:export": rank >= PERMISSION_LEVEL_ORDER.Write,
    "user_data:search:read": rank >= PERMISSION_LEVEL_ORDER.Read,
    "user_data:search:edit": rank >= PERMISSION_LEVEL_ORDER.Write,
    "user_data:search:delete": rank >= PERMISSION_LEVEL_ORDER.Delete,
  };
}

function genUserData(count = 30) {
  const users = [];
  for (let i = 0; i < count; i++) {
    const uuid = cryptoRandomUUID();
    const tenant = TENANTS[i % TENANTS.length];
    const label = `${tenant.toLowerCase()}_user_${i}`;
    const registration = new Date(2023 + (i % 3), (i * 3) % 12, (i % 27) + 1);
    const hadDeposit = i % 2 === 0;
    const fd = hadDeposit ? new Date(2024, (i * 2) % 12, (i % 27) + 1) : null;
    const depCount = hadDeposit ? (i % 5) + 1 : 0;
    const payCount = i % 3 === 0 ? (i % 4) : 0;
    const depSum = depCount * (50 + (i % 7) * 25);
    const paySum = payCount * (30 + (i % 5) * 20);
    const successRate = hadDeposit ? Math.max(0, Math.min(100, 40 + (i % 7) * 9 + (i % 3 ? -5 : 3))) : 0;
    const edge = i % 10 === 0;
    const labelScore = edge ? null : 500 - (i % 9) * 35;
    const marks = [];
    if (i % 6 === 0) marks.push("VIP");
    if (i % 7 === 0) marks.push("Suspicious");
    if (i % 11 === 0) marks.push("Risk Flag");
    const multi = i % 4 === 0;

    users.push({
      uuid,
      label,
      tenant,
      registrationDate: registration.toISOString(),
      firstDepositDate: fd ? fd.toISOString() : null,
      trafficSource: SOURCES[i % SOURCES.length],
      depositsCount: depCount,
      payoutsCount: payCount,
      depositsSum: depSum,
      payoutsSum: paySum,
      successDepositsPct: successRate,
      labelScore,
      marks,
      multi,
      linked: [],
      adjustments: 0,
      history: [],
    });
  }

  const pick = (arr) => arr[Math.floor(Math.random() * arr.length)];
  users.forEach((u, idx) => {
    if (!u.multi) return;

    const shared = users[(idx + 5) % users.length];
    const sharedUuid = shared.uuid;
    const sharedMarks = shared.marks?.length ? [...shared.marks] : [];

    u.linked.push({
      uuid: sharedUuid,
      label: shared.label,
      tenant: shared.tenant,
      labelScore: shared.labelScore,
      marks: sharedMarks,
      changeBy: "System",
      reason: buildRuleDetailById(RULE_IDS.PAYOUT_VELOCITY, idx),
    });

    const altLabel = `${shared.tenant.toLowerCase()}_linked_${idx}`;
    u.linked.push({
      uuid: sharedUuid,
      label: altLabel,
      tenant: shared.tenant,
      labelScore: typeof shared.labelScore === "number" ? Math.max(shared.labelScore - 25, 0) : null,
      marks: sharedMarks.length ? sharedMarks : ["Review"],
      changeBy: "System",
      reason: buildRuleDetailById(RULE_IDS.DUPLICATE_LABEL, idx + 1),
    });

    const extraLinks = 1 + (idx % 2);
    for (let j = 0; j < extraLinks; j++) {
      const t = pick(TENANTS);
      const aliasUuid = cryptoRandomUUID();
      const aliasLabel = `${t.toLowerCase()}_alias_${idx}_${j}`;
      const aliasScore = 460 - ((idx + j) % 9) * 18;
      const aliasMarks = (idx + j) % 3 === 0 ? ["Flagged"] : [];
      const manual = (idx + j) % 3 === 0;
      u.linked.push({
        uuid: aliasUuid,
        label: aliasLabel,
        tenant: t,
        labelScore: aliasScore,
        marks: aliasMarks,
        changeBy: manual ? "UserAdminPay" : "System",
        reason: manual
          ? buildTextDetail("Связь подтверждена вручную оператором AdminPay.")
          : buildRuleDetailById(RULE_IDS.VIP_HIGH_ROLLER, idx + j + 2),
      });
    }
  });

  if (users.length > 5) {
    const sameTenant = users[1].tenant;
    const duplicateLabel = `${sameTenant.toLowerCase()}_dupe_shared`;
    const duplicateUuid = cryptoRandomUUID();
    const duplicateReason = buildRuleDetailById(RULE_IDS.DUPLICATE_LABEL, 4);
    users[1].linked.push({
      uuid: duplicateUuid,
      label: duplicateLabel,
      tenant: sameTenant,
      labelScore: 380,
      marks: ["Review"],
      changeBy: "System",
      reason: duplicateReason,
    });
    users[4].linked.push({
      uuid: duplicateUuid,
      label: duplicateLabel,
      tenant: sameTenant,
      labelScore: 410,
      marks: [],
      changeBy: "System",
      reason: buildRuleDetailById(RULE_IDS.PAYOUT_VELOCITY, 5),
    });
  }

  const now = Date.now();
  users.forEach((u, idx) => {
    const baseScore = computeUuidScore(u);
    const baseTime = new Date(now - idx * 60 * 60 * 1000).toISOString();
    const followUpTime = new Date(now - idx * 60 * 60 * 1000 + 15 * 60 * 1000).toISOString();
    const marksList = u.marks?.length ? u.marks.join(", ") : "None";

    const initialEntry = makeHistoryEntry({
      event: HISTORY_EVENTS.RULE_APPLIED,
      details: buildRuleDetail(idx),
      previousValue: null,
      newValue: baseScore ?? null,
      when: baseTime,
    });

    const followUpEntry = u.marks?.length
      ? makeHistoryEntry({
          event: HISTORY_EVENTS.MARK_ADDED,
          details: buildTextDetail(`Marks synchronized: ${marksList}`),
          previousValue: baseScore ?? null,
          newValue: baseScore ?? null,
          when: followUpTime,
        })
      : makeHistoryEntry({
          event: HISTORY_EVENTS.RULE_RECALCULATION,
          details: buildTextDetail("Score validated during scheduled review."),
          previousValue: baseScore ?? null,
          newValue: baseScore ?? null,
          when: followUpTime,
        });

    u.history = [followUpEntry, initialEntry];
  });

  return users;
}

function computeUuidScore(user) {
  const scores = [];
  if (typeof user.labelScore === "number") scores.push(user.labelScore);
  user.linked.forEach((lk) => typeof lk.labelScore === "number" && scores.push(lk.labelScore));
  if (scores.length === 0) return null;
  const adjustment = Number.isFinite(Number(user.adjustments)) ? Number(user.adjustments) : 0;
  const mean = scores.reduce((a, b) => a + b, 0) / scores.length;
  return Math.round(mean + adjustment);
}

const mockDB = {
  users: genUserData(32),
};

const MockAPI = {
  listUsers: async (filters = {}, sort = null) => {
    const arr = mockDB.users.map((u) => ({ ...u }));

    const inRange = (v, from, to) => {
      if (from != null && v < from) return false;
      if (to != null && v > to) return false;
      return true;
    };

    const includesNeedle = (value, needle) =>
      value !== undefined && value !== null && String(value).toLowerCase().includes(needle);

    const dateInRange = (d, from, to) => {
      const val = d ? new Date(d).getTime() : null;
      if (from != null && (val == null || val < new Date(from).getTime())) return false;
      if (to != null && (val == null || val > new Date(to).getTime())) return false;
      return true;
    };

    let filtered = arr;

    if (filters.query) {
      const needle = filters.query.trim().toLowerCase();

      filtered = filtered.filter((u) => {
        const baseMatches =
          includesNeedle(u.uuid, needle) ||
          includesNeedle(u.label, needle) ||
          includesNeedle(u.tenant, needle) ||
          includesNeedle(u.trafficSource, needle) ||
          includesNeedle(u.registrationDate, needle) ||
          includesNeedle(u.firstDepositDate, needle) ||
          includesNeedle(u.labelScore, needle) ||
          includesNeedle(u.depositsCount, needle) ||
          includesNeedle(u.payoutsCount, needle) ||
          includesNeedle(u.depositsSum, needle) ||
          includesNeedle(u.payoutsSum, needle) ||
          includesNeedle(u.successDepositsPct, needle) ||
          includesNeedle(u.adjustments, needle) ||
          includesNeedle(computeUuidScore(u), needle) ||
          includesNeedle(u.linked?.length, needle) ||
          (u.marks?.some((mark) => includesNeedle(mark, needle)) ?? false);

        if (baseMatches) return true;

        const linkedMatches =
          u.linked?.some((lk) => {
            const reasonText =
              typeof lk.reason === "string"
                ? lk.reason
                : lk.reason?.text || lk.reason?.ruleName;
            return (
              includesNeedle(lk.uuid, needle) ||
              includesNeedle(lk.label, needle) ||
              includesNeedle(reasonText, needle) ||
              includesNeedle(lk.changeBy, needle) ||
              includesNeedle(lk.labelScore, needle) ||
              (lk.marks?.some((mark) => includesNeedle(mark, needle)) ?? false)
            );
          }) ?? false;

        if (linkedMatches) return true;

        const historyMatches =
          u.history?.some((entry) => {
            const detailText = entry.details?.text || entry.details?.ruleName;
            return (
              includesNeedle(entry.when, needle) ||
              includesNeedle(entry.event, needle) ||
              includesNeedle(detailText, needle) ||
              includesNeedle(entry.previousValue, needle) ||
              includesNeedle(entry.newValue, needle)
            );
          }) ?? false;

        return historyMatches;
      });
    }

    if (filters.searchValue) {
      const searchNeedle = filters.searchValue.trim().toLowerCase();
      filtered = filtered.filter((u) => {
        if (includesNeedle(u.uuid, searchNeedle)) return true;
        if (includesNeedle(u.label, searchNeedle)) return true;
        const linkedMatch =
          u.linked?.some(
            (lk) => includesNeedle(lk.uuid, searchNeedle) || includesNeedle(lk.label, searchNeedle)
          ) ?? false;
        return linkedMatch;
      });
    }
    if (filters.label) {
      const labelNeedle = filters.label.trim().toLowerCase();
      filtered = filtered.filter(
        (u) =>
          u.label.toLowerCase().includes(labelNeedle) ||
          u.linked?.some((lk) => lk.label?.toLowerCase().includes(labelNeedle))
      );
    }
    if (filters.tenant) filtered = filtered.filter((u) => u.tenant === filters.tenant);
    if (filters.riskFrom != null || filters.riskTo != null) {
      filtered = filtered.filter((u) => {
        const rs = u.labelScore;
        if (rs == null) return false;
        return inRange(rs, filters.riskFrom, filters.riskTo);
      });
    }
    if (filters.fdFrom != null || filters.fdTo != null) filtered = filtered.filter((u) => dateInRange(u.firstDepositDate, filters.fdFrom, filters.fdTo));
    if (filters.regFrom != null || filters.regTo != null) filtered = filtered.filter((u) => dateInRange(u.registrationDate, filters.regFrom, filters.regTo));
    if (filters.source) filtered = filtered.filter((u) => u.trafficSource === filters.source);
    if (filters.depCountFrom != null || filters.depCountTo != null) filtered = filtered.filter((u) => inRange(u.depositsCount, filters.depCountFrom, filters.depCountTo));
    if (filters.payCountFrom != null || filters.payCountTo != null) filtered = filtered.filter((u) => inRange(u.payoutsCount, filters.payCountFrom, filters.payCountTo));
    if (filters.depSumFrom != null || filters.depSumTo != null) filtered = filtered.filter((u) => inRange(u.depositsSum, filters.depSumFrom, filters.depSumTo));
    if (filters.paySumFrom != null || filters.paySumTo != null) filtered = filtered.filter((u) => inRange(u.payoutsSum, filters.paySumFrom, filters.paySumTo));
    if (filters.successFrom != null || filters.successTo != null) filtered = filtered.filter((u) => inRange(u.successDepositsPct, filters.successFrom, filters.successTo));

    if (sort === "risk_desc") filtered.sort((a, b) => (computeUuidScore(b) ?? -Infinity) - (computeUuidScore(a) ?? -Infinity));
    return new Promise((r) => setTimeout(() => r(filtered), 250));
  },
  exportCSV: async (rows) => {
    const cols = LIST_COLUMNS;
    const lines = [cols.join(",")];
    rows.forEach((u) => {
      const vals = [
        u.uuid,
        u.label,
        u.tenant,
        u.labelScore ?? "",
        fmtDate(u.firstDepositDate),
        fmtDate(u.registrationDate),
        u.trafficSource,
        u.depositsCount,
        u.payoutsCount,
        u.depositsSum,
        u.payoutsSum,
        u.successDepositsPct,
        u.linked?.length ?? 0,
      ];
      lines.push(vals.map((v) => (typeof v === "string" && v.includes(",") ? `"${v}"` : v)).join(","));
    });
    const blob = new Blob([lines.join("\n")], { type: "text/csv;charset=utf-8;" });
    const url = URL.createObjectURL(blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = `users_export_${Date.now()}.csv`;
    a.click();
    URL.revokeObjectURL(url);
    return true;
  },
  findUserByUUID: async (uuid) => mockDB.users.find((u) => u.uuid === uuid) || null,
  findUserByLabel: async (tenant, label) => {
    if (!label) return null;
    const direct = mockDB.users.find(
      (u) => u.label === label && (!tenant || u.tenant === tenant)
    );
    if (direct) return direct;
    const linkedHost = mockDB.users.find((u) =>
      u.linked?.some((lk) => lk.label === label && (!tenant || lk.tenant === tenant))
    );
    return linkedHost || null;
  },
  saveUser: async (user) => {
    const idx = mockDB.users.findIndex((x) => x.uuid === user.uuid);
    if (idx >= 0) mockDB.users[idx] = { ...user };
    return true;
  },
  attachLabelToUUID: async (targetUUID, { tenant, label, reason, labelScore = null }) => {
    const target = mockDB.users.find((u) => u.uuid === targetUUID);
    if (!target) return { ok: false, message: "UUID not found" };

    const other = mockDB.users.find(
      (u) =>
        u.uuid !== targetUUID &&
        ((u.label === label && u.tenant === tenant) || u.linked.some((lk) => lk.tenant === tenant && lk.label === label))
    );

    if (other && other.tenant === tenant) {
      const prevScore = computeUuidScore(target);
      const moved = [
        {
          uuid: other.uuid,
          label: other.label,
          tenant: other.tenant,
          labelScore: other.labelScore,
          marks: other.marks,
          changeBy: "System",
          reason: normalizeReason(other.reason, 0),
        },
        ...other.linked.map((lk, idx) => ({
          ...lk,
          changeBy: normalizeChangeBy(lk.changeBy),
          reason: normalizeReason(lk.reason, idx + 1),
        })),
      ];
      target.linked.push(...moved);
      mockDB.users = mockDB.users.filter((u) => u.uuid !== other.uuid);

      const newScore = computeUuidScore(target);
      target.history.unshift(
        makeHistoryEntry({
          event: HISTORY_EVENTS.LABEL_LINKED,
          details: buildRuleDetailById(RULE_IDS.DUPLICATE_LABEL, 0),
          previousValue: prevScore,
          newValue: newScore,
        })
      );
    } else {
      const prevScore = computeUuidScore(target);
      const linkReason = normalizeReason(reason || "Manual attachment", 1);
      const newLink = {
        uuid: cryptoRandomUUID(),
        label,
        tenant,
        labelScore,
        marks: [],
        changeBy: "UserAdminPay",
        reason: linkReason,
      };
      target.linked.push(newLink);
      const newScore = computeUuidScore(target);
      target.history.unshift(
        makeHistoryEntry({
          event: HISTORY_EVENTS.LABEL_LINKED,
          details: linkReason,
          previousValue: prevScore,
          newValue: newScore,
        })
      );
    }

    await MockAPI.saveUser(target);
    return { ok: true, target };
  },
  unlinkLabelFromUUID: async (uuid, label) => {
    const user = mockDB.users.find((u) => u.uuid === uuid);
    if (!user) return { ok: false, message: "UUID not found" };
    const link = user.linked.find((lk) => lk.label === label);
    if (!link) return { ok: false, message: "Label not linked" };

    const before = computeUuidScore(user);
    user.linked = user.linked.filter((lk) => lk.label !== label);
    const after = computeUuidScore(user);

    const unlinkDetail = normalizeReason(link.reason, 2);
    user.history.unshift(
      makeHistoryEntry({
        event: HISTORY_EVENTS.LABEL_UNLINKED,
        details: unlinkDetail,
        previousValue: before,
        newValue: after,
      })
    );

    if (before !== after) {
      user.history.unshift(
        makeHistoryEntry({
          event: HISTORY_EVENTS.RULE_RECALCULATION,
          details: buildTextDetail(`Risk score recalculated after unlinking ${label}.`),
          previousValue: before,
          newValue: after,
        })
      );
    }

    const stillLinked = mockDB.users.some((candidate) => {
      if (candidate.uuid === user.uuid) return false;
      if (candidate.tenant !== link.tenant) return false;
      if (candidate.label === label) return true;
      return candidate.linked.some((lk) => lk.label === label && lk.tenant === link.tenant);
    });

    let created = null;
    if (!stillLinked) {
      created = {
        uuid: cryptoRandomUUID(),
        label,
        tenant: link.tenant,
        registrationDate: new Date().toISOString(),
        firstDepositDate: null,
        trafficSource: "Manual",
        depositsCount: 0,
        payoutsCount: 0,
        depositsSum: 0,
        payoutsSum: 0,
        successDepositsPct: 0,
        labelScore: link.labelScore ?? null,
        marks: link.marks ? [...link.marks] : [],
        multi: false,
        linked: [],
        adjustments: 0,
        history: [],
      };
      const createdScore = computeUuidScore(created);
      created.history.unshift(
        makeHistoryEntry({
          event: HISTORY_EVENTS.LABEL_LINKED,
          details: buildTextDetail(`Label isolated from UUID ${uuid}.`),
          previousValue: null,
          newValue: createdScore,
        })
      );
      mockDB.users.push(created);
    }

    await MockAPI.saveUser(user);
    return { ok: true, user, created };
  },
};

function Tag({ children, onRemove }) {
  return (
    <span className="mr-1 mb-1 inline-flex items-center rounded-full border px-2 py-0.5 text-xs">
      <span>{children}</span>
      {typeof onRemove === "function" && (
        <button
          type="button"
          className="ml-1 inline-flex h-4 w-4 items-center justify-center rounded-full border border-transparent text-[10px] leading-none text-gray-500 hover:border-gray-400 hover:text-black"
          onClick={onRemove}
          aria-label={`Remove ${children}`}
        >
          ×
        </button>
      )}
    </span>
  );
}

function Modal({ open, onClose, title, children, footer }) {
  if (!open) return null;
  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-black/40" onClick={onClose}>
      <div className="w-full max-w-3xl rounded-2xl bg-white shadow-xl" onClick={(e) => e.stopPropagation()}>
        <div className="flex items-center justify-between border-b px-4 py-3">
          <h3 className="text-lg font-semibold">{title}</h3>
          <button className="px-2 text-gray-500 hover:text-black" onClick={onClose}>
            ✕
          </button>
        </div>
        <div className="max-h-[60vh] overflow-auto p-4">{children}</div>
        {footer !== undefined && (
          <div className="flex items-center justify-between border-t px-4 py-3">{footer}</div>
        )}
      </div>
    </div>
  );
}

function Toast({ text, onHide }) {
  if (!text) return null;
  return (
    <div
      className="fixed bottom-4 left-1/2 z-50 -translate-x-1/2 rounded-xl bg-black px-4 py-2 text-white shadow-lg"
      onAnimationEnd={onHide}
    >
      {text}
    </div>
  );
}

function SimpleSelect({ options, value, onChange, placeholder }) {
  const [open, setOpen] = React.useState(false);
  return (
    <div className="relative">
      <button type="button" className="mt-1 w-full rounded-xl border px-3 py-2 text-left" onClick={() => setOpen((o) => !o)}>
        {value || placeholder || "Select"}
      </button>
      {open && (
        <div className="absolute z-10 mt-1 w-full rounded-xl border bg-white shadow">
          <div className="max-h-56 overflow-auto">
            {options.map((opt) => (
              <div
                key={opt}
                className="cursor-pointer px-3 py-2 hover:bg-gray-100"
                onClick={() => {
                  onChange(opt);
                  setOpen(false);
                }}
              >
                {opt}
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}

function SearchableSelect({ options, value, onChange, placeholder }) {
  const [open, setOpen] = React.useState(false);
  const [q, setQ] = React.useState("");
  const filtered = useMemo(() => options.filter((o) => o.toLowerCase().includes(q.toLowerCase())), [options, q]);
  return (
    <div className="relative">
      <button type="button" className="mt-1 w-full rounded-xl border px-3 py-2 text-left" onClick={() => setOpen((o) => !o)}>
        {value || placeholder || "Select"}
      </button>
      {open && (
        <div className="absolute z-10 mt-1 w-full rounded-xl border bg-white shadow">
          <div className="p-2 border-b">
            <input className="w-full rounded-lg border px-2 py-1" placeholder="Search…" value={q} onChange={(e) => setQ(e.target.value)} />
          </div>
          <div className="max-h-56 overflow-auto">
            {filtered.map((opt) => (
              <div
                key={opt}
                className="cursor-pointer px-3 py-2 hover:bg-gray-100"
                onClick={() => {
                  onChange(opt);
                  setOpen(false);
                }}
              >
                {opt}
              </div>
            ))}
            {!filtered.length && <div className="px-3 py-2 text-sm text-gray-500">No results</div>}
          </div>
        </div>
      )}
    </div>
  );
}

function FiltersModal({ open, onClose, onApply, value }) {
  const [loc, setLoc] = useState(value);
  React.useEffect(() => {
    setLoc(value);
  }, [value]);

  const update = (patch) => setLoc((s) => ({ ...s, ...patch }));
  const num = (v) => (v === "" || v === null || v === undefined ? undefined : Number(v));

  const validateRange = (from, to) => from != null && to != null && Number(from) > Number(to);

  const invalidRange =
    validateRange(loc?.riskFrom, loc?.riskTo) ||
    validateRange(loc?.depCountFrom, loc?.depCountTo) ||
    validateRange(loc?.payCountFrom, loc?.payCountTo) ||
    validateRange(loc?.depSumFrom, loc?.depSumTo) ||
    validateRange(loc?.paySumFrom, loc?.paySumTo) ||
    validateRange(loc?.successFrom, loc?.successTo) ||
    validateRange(loc?.fdFrom, loc?.fdTo) ||
    validateRange(loc?.regFrom, loc?.regTo);

  const successOutOfBounds =
    (loc?.successFrom != null && (loc.successFrom < 0 || loc.successFrom > 100)) ||
    (loc?.successTo != null && (loc.successTo < 0 || loc.successTo > 100));

  const disabledApply = invalidRange || successOutOfBounds;

  const handleApply = () => {
    const cleaned = Object.fromEntries(
      Object.entries(loc || {}).filter(([, v]) => v !== undefined && v !== "" && !(Array.isArray(v) && v.length === 0))
    );
    onApply(cleaned);
  };

  return (
    <Modal
      open={open}
      onClose={onClose}
      title="User Filters"
      footer={
        <>
          <button className="rounded-xl border px-4 py-2" onClick={onClose}>
            Close
          </button>
          <button className="rounded-xl bg-black px-4 py-2 text-white disabled:opacity-50" disabled={disabledApply} onClick={handleApply}>
            Apply
          </button>
        </>
      }
    >
      <div className="grid gap-4 md:grid-cols-2">
        <div className="grid grid-cols-2 gap-2">
          <div>
          <label className="block text-sm font-medium">Risk Score (Label) — From</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.riskFrom ?? ""}
              onChange={(e) => update({ riskFrom: num(e.target.value) })}
              placeholder="e.g., -100"
            />
          </div>
          <div>
          <label className="block text-sm font-medium">Risk Score (Label) — To</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.riskTo ?? ""}
              onChange={(e) => update({ riskTo: num(e.target.value) })}
              placeholder="e.g., 800"
            />
          </div>
        </div>
        <div>
          <label className="block text-sm font-medium">Tenant</label>
          <SimpleSelect options={TENANTS} value={loc?.tenant || ""} onChange={(v) => update({ tenant: v })} placeholder="Select tenant…" />
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Date FD — From</label>
            <input
              type="date"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.fdFrom || ""}
              onChange={(e) => update({ fdFrom: e.target.value || undefined })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Date FD — To</label>
            <input
              type="date"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.fdTo || ""}
              onChange={(e) => update({ fdTo: e.target.value || undefined })}
            />
          </div>
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Registration Date — From</label>
            <input
              type="date"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.regFrom || ""}
              onChange={(e) => update({ regFrom: e.target.value || undefined })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Registration Date — To</label>
            <input
              type="date"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.regTo || ""}
              onChange={(e) => update({ regTo: e.target.value || undefined })}
            />
          </div>
        </div>
        <div>
          <label className="block text-sm font-medium">Source of Traffic</label>
          <SearchableSelect options={SOURCES} value={loc?.source || ""} onChange={(v) => update({ source: v })} placeholder="Select source…" />
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Deposits Count — From</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.depCountFrom ?? ""}
              onChange={(e) => update({ depCountFrom: num(e.target.value) })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Deposits Count — To</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.depCountTo ?? ""}
              onChange={(e) => update({ depCountTo: num(e.target.value) })}
            />
          </div>
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Payouts Count — From</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.payCountFrom ?? ""}
              onChange={(e) => update({ payCountFrom: num(e.target.value) })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Payouts Count — To</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.payCountTo ?? ""}
              onChange={(e) => update({ payCountTo: num(e.target.value) })}
            />
          </div>
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Deposits Sum — From</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.depSumFrom ?? ""}
              onChange={(e) => update({ depSumFrom: num(e.target.value) })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Deposits Sum — To</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.depSumTo ?? ""}
              onChange={(e) => update({ depSumTo: num(e.target.value) })}
            />
          </div>
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Payouts Sum — From</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.paySumFrom ?? ""}
              onChange={(e) => update({ paySumFrom: num(e.target.value) })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Payouts Sum — To</label>
            <input
              type="number"
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.paySumTo ?? ""}
              onChange={(e) => update({ paySumTo: num(e.target.value) })}
            />
          </div>
        </div>
        <div className="grid grid-cols-2 gap-2">
          <div>
            <label className="block text-sm font-medium">Success Deposits % — From</label>
            <input
              type="number"
              min={0}
              max={100}
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.successFrom ?? ""}
              onChange={(e) => update({ successFrom: num(e.target.value) })}
            />
          </div>
          <div>
            <label className="block text-sm font-medium">Success Deposits % — To</label>
            <input
              type="number"
              min={0}
              max={100}
              className="mt-1 w-full rounded-xl border px-3 py-2"
              value={loc?.successTo ?? ""}
              onChange={(e) => update({ successTo: num(e.target.value) })}
            />
          </div>
        </div>
        {disabledApply && (
          <div className="md:col-span-2 rounded-xl bg-rose-50 px-3 py-2 text-sm text-rose-600">
            Check that "From" is not greater than "To" and Success % stays within 0-100.
          </div>
        )}
      </div>
    </Modal>
  );
}

function ListView({ perms, onOpenRule, onOpenUser, refreshSignal }) {
  const [baseFilters, setBaseFilters] = useState(() => ({}));
  const [searchFilters, setSearchFilters] = useState(() => ({}));
  const [showFilters, setShowFilters] = useState(false);
  const [toast, setToast] = useState("");
  const [rows, setRows] = useState([]);
  const [loaded, setLoaded] = useState(false);
  const [searchTerm, setSearchTerm] = useState("");
  const toastTimeout = React.useRef(null);
  const canReadList = perms["user_data:list:read"];
  const canRefresh = perms["user_data:list:refresh"];
  const canExport = perms["user_data:list:export"];
  const canReadUser = perms["user_data:search:read"];

  const combinedFilters = useMemo(
    () => ({ ...baseFilters, ...searchFilters }),
    [baseFilters, searchFilters]
  );

  const searchPlaceholder = useMemo(() => "Search by UUID or Label…", []);

  const showToast = React.useCallback((message, duration = 1500) => {
    if (!message) return;
    setToast(message);
    window.clearTimeout(toastTimeout.current);
    toastTimeout.current = window.setTimeout(() => setToast(""), duration);
  }, []);

  const refresh = React.useCallback(async ({ notify } = {}) => {
    if (!canReadList) {
      setRows([]);
      setLoaded(true);
      return;
    }
    const res = await MockAPI.listUsers(combinedFilters, "risk_desc");
    setRows(res);
    setLoaded(true);
    if (notify) {
      showToast("Data refreshed.");
    }
  }, [combinedFilters, showToast, canReadList]);

  React.useEffect(() => {
    refresh();
  }, [refresh]);

  const refreshReadyRef = React.useRef(false);
  React.useEffect(() => {
    if (!refreshReadyRef.current) {
      refreshReadyRef.current = true;
      return;
    }
    refresh();
  }, [refreshSignal, refresh]);

  React.useEffect(
    () => () => {
      window.clearTimeout(toastTimeout.current);
    },
    []
  );

  const filtersApplied = useMemo(
    () => Object.keys(combinedFilters).length > 0,
    [combinedFilters]
  );

  const handleReset = () => {
    setBaseFilters({});
    setSearchFilters({});
    setSearchTerm("");
    showToast("Filters cleared.");
  };

  const handleExport = () => {
    if (!canExport || !filtersApplied) return;
    MockAPI.exportCSV(rows).then(() => showToast("Export completed."));
  };

  const sanitizeFilters = React.useCallback((candidate) =>
    Object.fromEntries(
      Object.entries(candidate || {}).filter(([, value]) =>
        value !== undefined && value !== "" && !(Array.isArray(value) && value.length === 0)
      )
    ),
  []);

  const handleApplyFilters = (candidate) => {
    setShowFilters(false);
    setBaseFilters(sanitizeFilters(candidate));
  };

  const openUserCard = React.useCallback(
    (loader) => {
      if (!canReadUser) {
        showToast("Read permission required to open user card.", 1800);
        return;
      }
      if (typeof onOpenUser === "function") {
        onOpenUser(loader, {
          onNotFound: () => showToast("User not found.", 1800),
        });
      }
    },
    [canReadUser, onOpenUser, showToast]
  );

  const handleOpenLabel = (tenant, label) => {
    openUserCard(MockAPI.findUserByLabel(tenant, label));
  };

  const handleSearch = () => {
    const trimmed = searchTerm.trim();
    if (!trimmed) {
      showToast("Enter a search value.", 1600);
      return;
    }
    setSearchFilters({ searchValue: trimmed });
  };

  const clearSearch = () => {
    setSearchFilters({});
    setSearchTerm("");
  };

  const searchDisabled = useMemo(() => !searchTerm.trim(), [searchTerm]);

  const renderCell = (u, column) => {
    switch (column) {
      case "UUID":
        return <span className="font-mono text-xs text-slate-600">{u.uuid}</span>;
      case "Label":
        return (
          <button
            type="button"
            className="text-blue-700 underline"
            onClick={() => handleOpenLabel(u.tenant, u.label)}
          >
            {u.label}
          </button>
        );
      case "Tenant":
        return u.tenant;
      case "Risk Score (Label)":
        return u.labelScore ?? "–";
      case "FD":
        return fmtDate(u.firstDepositDate);
      case "Registration Date":
        return fmtDate(u.registrationDate);
      case "Source of Traffic":
        return u.trafficSource;
      case "Deposits Count":
        return u.depositsCount;
      case "Payouts Count":
        return u.payoutsCount;
      case "Deposits Sum":
        return fmtMoney(u.depositsSum);
      case "Payouts Sum":
        return fmtMoney(u.payoutsSum);
      case "Success Deposits %":
        return fmtPct(u.successDepositsPct);
      case "Multi-account":
        return u.linked?.length ?? 0;
      default:
        return "";
    }
  };

  const noAccess = !canReadList;

  return (
    <div>
      {noAccess ? (
        <div className="rounded-2xl border p-4 text-sm text-gray-600">
          You do not have permission to view the user list.
        </div>
      ) : (
        <>
          <div className="mb-3 flex flex-wrap items-center gap-2">
            <button
              className="rounded-xl border px-3 py-2 disabled:cursor-not-allowed disabled:opacity-50"
              onClick={canRefresh ? () => refresh({ notify: true }) : undefined}
              disabled={!canRefresh}
              title={canRefresh ? undefined : "Write permission required"}
            >
              Refresh
            </button>
            <button className="rounded-xl border px-3 py-2" onClick={() => setShowFilters(true)}>
              Filters
            </button>
            <button className="rounded-xl border px-3 py-2 disabled:opacity-50" disabled={!filtersApplied} onClick={handleReset}>
              Reset
            </button>
            {canExport && filtersApplied ? (
              <button className="rounded-xl border px-3 py-2" onClick={handleExport}>
                CSV
              </button>
            ) : (
              filtersApplied && !canExport && (
                <span className="text-sm text-gray-500">CSV hidden (write permission required)</span>
              )
            )}
          </div>
          <div className="mb-4 flex flex-wrap gap-3 rounded-2xl border px-4 py-3">
            <div className="flex-1 min-w-[260px]">
              <label className="block text-sm font-medium">Search users</label>
              <input
                className="mt-1 w-full rounded-xl border px-3 py-2"
                value={searchTerm}
                onChange={(e) => setSearchTerm(e.target.value)}
                onKeyDown={(e) => {
                  if (e.key === "Enter") handleSearch();
                }}
                placeholder={searchPlaceholder}
              />
            </div>

            <div className="flex items-end gap-2">
              <button
                className="rounded-xl border px-3 py-2 disabled:cursor-not-allowed disabled:opacity-50"
                onClick={handleSearch}
                disabled={searchDisabled}
              >
                Search
              </button>
              <button className="rounded-xl border px-3 py-2" onClick={clearSearch}>
                Clear Search
              </button>
            </div>
          </div>

          <div className="overflow-auto rounded-2xl border">
            <table className="min-w-[1000px] w-full text-sm">
              <thead className="bg-gray-50">
                <tr>
                  {LIST_COLUMNS.map((h) => (
                    <th key={h} className="whitespace-nowrap px-3 py-2 text-left font-medium">
                      {h}
                    </th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {rows.map((u) => (
                  <tr key={u.uuid} className="border-t">
                    {LIST_COLUMNS.map((column) => (
                      <td key={`${u.uuid}-${column}`} className="px-3 py-2">
                        {renderCell(u, column)}
                      </td>
                    ))}
                  </tr>
                ))}
                {!loaded && (
                  <tr>
                    <td colSpan={LIST_COLUMNS.length} className="px-3 py-6 text-center text-sm text-gray-500">
                      Loading…
                    </td>
                  </tr>
                )}
                {loaded && !rows.length && (
                  <tr>
                    <td colSpan={LIST_COLUMNS.length} className="px-3 py-6 text-center text-sm text-gray-500">
                      No data found.
                    </td>
                  </tr>
                )}
              </tbody>
            </table>
          </div>
        </>
      )}

      <FiltersModal
        open={!noAccess && showFilters}
        value={baseFilters}
        onClose={() => setShowFilters(false)}
        onApply={handleApplyFilters}
      />

      <Toast text={toast} onHide={() => setToast("")} />
    </div>
  );
}

function AddLabelModal({ open, onClose, onSave, defaultTenant }) {
  const [tenant, setTenant] = useState(defaultTenant || TENANTS[0]);
  const [label, setLabel] = useState("");
  const [reason, setReason] = useState("");

  React.useEffect(() => {
    if (open) {
      setTenant(defaultTenant || TENANTS[0]);
      setLabel("");
      setReason("");
    }
  }, [defaultTenant, open]);

  return (
    <Modal
      open={open}
      onClose={onClose}
      title="Attach Label to UUID"
      footer={
        <>
          <button className="rounded-xl border px-4 py-2" onClick={onClose}>
            Cancel
          </button>
          <button className="rounded-xl bg-black px-4 py-2 text-white" onClick={() => onSave({ tenant, label, reason })} disabled={!label.trim()}>
            Save
          </button>
        </>
      }
    >
      <div className="grid gap-3">
        <div>
          <label className="block text-sm font-medium">Label</label>
          <input
            className="mt-1 w-full rounded-xl border px-3 py-2"
            value={label}
            onChange={(e) => setLabel(e.target.value)}
            placeholder="Enter Label…"
          />
        </div>
        <div>
          <label className="block text-sm font-medium">Tenant</label>
          <select className="mt-1 w-full rounded-xl border px-3 py-2" value={tenant} onChange={(e) => setTenant(e.target.value)}>
            {TENANTS.map((t) => (
              <option key={t} value={t}>
                {t}
              </option>
            ))}
          </select>
        </div>
        <div>
          <label className="block text-sm font-medium">Reason (Manual)</label>
          <textarea
            className="mt-1 w-full rounded-xl border px-3 py-2"
            rows={3}
            value={reason}
            onChange={(e) => setReason(e.target.value)}
            placeholder="e.g., Same IP, payout details, device fingerprint…"
          />
        </div>
      </div>
    </Modal>
  );
}

function UserCard({ user, perms, onChange, onOpenLabel, onOpenRule }) {
  const [mode, setMode] = useState("view");
  const [edit, setEdit] = useState({ ...user });
  const [toast, setToast] = useState("");
  const [showAdd, setShowAdd] = useState(false);
  const [adjustmentInput, setAdjustmentInput] = useState(String(user.adjustments ?? 0));
  const [markDraft, setMarkDraft] = useState("");
  const toastTimeout = React.useRef(null);
  const canEdit = perms["user_data:search:edit"];
  const canDelete = perms["user_data:search:delete"];

  React.useEffect(() => {
    setEdit({ ...user });
    setMode("view");
    setAdjustmentInput(String(user.adjustments ?? 0));
  }, [user]);

  React.useEffect(() => {
    if (!canEdit && mode === "edit") {
      setMode("view");
    }
  }, [canEdit, mode]);

  React.useEffect(() => {
    if (!canEdit) {
      setShowAdd(false);
    }
  }, [canEdit]);

  React.useEffect(
    () => () => {
      window.clearTimeout(toastTimeout.current);
    },
    []
  );

  const showToast = React.useCallback((message, duration = 1500) => {
    if (!message) return;
    setToast(message);
    window.clearTimeout(toastTimeout.current);
    toastTimeout.current = window.setTimeout(() => setToast(""), duration);
  }, []);

  const uuidScore = useMemo(() => {
    const numericAdj = adjustmentInput.trim() === "" ? 0 : Number(adjustmentInput);
    return computeUuidScore({ ...edit, adjustments: numericAdj });
  }, [adjustmentInput, edit]);

  const availableMarks = useMemo(
    () => SYSTEM_MARKS.filter((mark) => !(edit.marks || []).includes(mark)),
    [edit.marks]
  );

  React.useEffect(() => {
    if (markDraft && !availableMarks.includes(markDraft)) {
      setMarkDraft("");
    }
  }, [availableMarks, markDraft]);

  const validateAdj = (value) => {
    if (!value.trim()) return true;
    if (!/^[-+]?\d+$/.test(value.trim())) return "Adjustment value must be a number.";
    const n = Number(value);
    if (n < -1000 || n > 1000) return "Adjustment out of allowed range (-1000…+1000).";
    return true;
  };

  const save = () => {
    if (!canEdit) return;
    const validation = validateAdj(adjustmentInput);
    if (validation !== true) {
      showToast(validation, 1800);
      return;
    }
    const numericAdj = adjustmentInput.trim() === "" ? 0 : Number(adjustmentInput);
    const prevAdj = Number(edit.adjustments ?? 0);
    const prevScore = computeUuidScore({ ...edit, adjustments: prevAdj });
    const newScore = computeUuidScore({ ...edit, adjustments: numericAdj });
    let history = edit.history || [];
    if (numericAdj !== prevAdj) {
      const delta = numericAdj - prevAdj;
      const text = `Manual adjustment ${delta >= 0 ? "+" : ""}${delta}`;
      history = [
        makeHistoryEntry({
          event: HISTORY_EVENTS.MANUAL_ADJUSTMENT,
          details: buildTextDetail(text),
          previousValue: prevScore,
          newValue: newScore,
        }),
        ...history,
      ];
    }
    const next = { ...edit, adjustments: numericAdj, history };
    MockAPI.saveUser(next).then(() => {
      setMode("view");
      setEdit(next);
      showToast("User data successfully updated.");
      onChange?.(next);
    });
  };

  const unlink = (label) => {
    if (!canDelete) return;
    if (!window.confirm("Are you sure you want to unlink this Label from the UUID?")) return;
    MockAPI.unlinkLabelFromUUID(edit.uuid, label).then(({ user: updated, created }) => {
      if (updated) {
        setEdit({ ...updated });
        onChange?.({ ...updated });
      }
      if (created) {
        showToast(`Label ${label} unlinked. New UUID ${created.uuid} created automatically.`);
      } else {
        showToast(`Label ${label} unlinked.`);
      }
    });
  };

  const handleAddMark = (value) => {
    if (!canEdit) return;
    const trimmed = value.trim();
    if (!trimmed) return;
    if (!SYSTEM_MARKS.includes(trimmed)) {
      showToast("Selected mark is not available.", 1800);
      return;
    }
    let added = false;
    let already = false;
    setEdit((prev) => {
      if ((prev.marks || []).includes(trimmed)) {
        already = true;
        return prev;
      }
      const nextMarks = [...(prev.marks || []), trimmed];
      const entry = makeHistoryEntry({
        event: HISTORY_EVENTS.MARK_ADDED,
        details: buildTextDetail(`Mark added: ${trimmed}`),
        previousValue: computeUuidScore(prev),
        newValue: computeUuidScore({ ...prev, marks: nextMarks }),
      });
      added = true;
      return { ...prev, marks: nextMarks, history: [entry, ...(prev.history || [])] };
    });
    if (already) {
      showToast("Mark already added.", 1500);
      return;
    }
    if (added) {
      showToast(`Mark "${trimmed}" added.`);
      setMarkDraft("");
    }
  };
  const handleClearMarks = () => {
    if (!canEdit) return;
    if (!(edit.marks || []).length) return;
    setEdit((prev) => {
      if (!(prev.marks || []).length) return prev;
      const entry = makeHistoryEntry({
        event: HISTORY_EVENTS.MARK_REMOVED,
        details: buildTextDetail("All marks cleared."),
        previousValue: computeUuidScore(prev),
        newValue: computeUuidScore({ ...prev, marks: [] }),
      });
      return { ...prev, marks: [], history: [entry, ...(prev.history || [])] };
    });
    showToast("Marks cleared.");
    setMarkDraft("");
  };

  const handleRemoveMark = (mark) => {
    if (!canEdit) return;
    if (!(edit.marks || []).includes(mark)) return;
    let removed = false;
    setEdit((prev) => {
      if (!(prev.marks || []).includes(mark)) return prev;
      const nextMarks = (prev.marks || []).filter((item) => item !== mark);
      const entry = makeHistoryEntry({
        event: HISTORY_EVENTS.MARK_REMOVED,
        details: buildTextDetail(`Mark removed: ${mark}`),
        previousValue: computeUuidScore(prev),
        newValue: computeUuidScore({ ...prev, marks: nextMarks }),
      });
      removed = true;
      return { ...prev, marks: nextMarks, history: [entry, ...(prev.history || [])] };
    });
    if (removed) {
      setMarkDraft((current) => (current === mark ? "" : current));
      showToast(`Mark "${mark}" removed.`);
    }
  };

  return (
    <div className="rounded-2xl border p-4">
      <div className="mb-4 flex items-start justify-between">
        <div>
          <div className="text-xs uppercase text-gray-500">UUID</div>
          <div className="font-mono text-lg">{edit.uuid}</div>
        </div>
        {canEdit && (
          <button title="Switch to edit mode." className="rounded-xl border px-3 py-2" onClick={() => setMode((m) => (m === "view" ? "edit" : "view"))}>
            {mode === "view" ? "Edit" : "View"}
          </button>
        )}
      </div>

      <div className="grid gap-4 md:grid-cols-2">
        <div>
          <div className="text-sm text-gray-500">Label</div>
          <div className="font-medium">{edit.label}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Tenant</div>
          <div className="font-medium">{edit.tenant}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">UUID Score</div>
          <div className="font-medium">{uuidScore ?? "–"}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Label Score</div>
          <div className="font-medium">{edit.labelScore ?? "–"}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">FD (First Deposit Date)</div>
          <div className="font-medium">{fmtDate(edit.firstDepositDate)}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Registration Date</div>
          <div className="font-medium">{fmtDate(edit.registrationDate)}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Source of Traffic</div>
          <div className="font-medium">{edit.trafficSource}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Deposits Count</div>
          <div className="font-medium">{edit.depositsCount}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Payouts Count</div>
          <div className="font-medium">{edit.payoutsCount}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Deposits Sum</div>
          <div className="font-medium">{fmtMoney(edit.depositsSum)}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Payouts Sum</div>
          <div className="font-medium">{fmtMoney(edit.payoutsSum)}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Success Deposits %</div>
          <div className="font-medium">{fmtPct(edit.successDepositsPct)}</div>
        </div>
        <div>
          <div className="text-sm text-gray-500">Mark</div>
          <div>
            <div className="flex flex-wrap items-center">
              {edit.marks?.length ? (
                edit.marks.map((m, i) => (
                  <Tag key={`${m}-${i}`} onRemove={mode === "edit" && canEdit ? () => handleRemoveMark(m) : undefined}>
                    {m}
                  </Tag>
                ))
              ) : (
                <span>–</span>
              )}
            </div>
            {mode === "edit" && canEdit && (
              <div className="mt-2 flex flex-wrap items-center gap-2">
                <select
                  className="w-56 rounded-xl border px-3 py-2"
                  value={markDraft}
                  onChange={(e) => setMarkDraft(e.target.value)}
                  disabled={!availableMarks.length}
                >
                  <option value="">Select mark…</option>
                  {availableMarks.map((mark) => (
                    <option key={mark} value={mark}>
                      {mark}
                    </option>
                  ))}
                </select>
                <button
                  className="rounded-xl border px-3 py-2"
                  onClick={() => handleAddMark(markDraft)}
                  disabled={!markDraft}
                >
                  Add
                </button>
                <button className="rounded-xl border px-3 py-2" onClick={handleClearMarks}>
                  Clear
                </button>
                {!availableMarks.length && <span className="text-sm text-gray-500">All system marks applied.</span>}
              </div>
            )}
          </div>
        </div>
        {mode === "edit" && canEdit && (
          <div className="md:col-span-2">
            <div className="text-sm text-gray-500">Risk Score Adjustment (−1000…+1000)</div>
            <div className="mt-1 flex items-center gap-2">
              <input
                className="w-40 rounded-xl border px-3 py-2"
                value={adjustmentInput}
                onChange={(e) => setAdjustmentInput(e.target.value)}
              />
              <span className="text-sm text-gray-500">
                UUID Score ⇒ <b>{uuidScore ?? "–"}</b>
              </span>
            </div>
          </div>
        )}
      </div>

      <div className="mt-6">
        <div className="mb-2 flex items-center justify-between">
          <h4 className="text-base font-semibold">Linked Accounts ({edit.linked?.length || 0})</h4>
          {canEdit && mode === "edit" && (
            <button className="rounded-xl border px-3 py-2" onClick={() => setShowAdd(true)}>
              Add Label
            </button>
          )}
        </div>
        {edit.linked?.length ? (
          <div className="overflow-auto rounded-xl border">
            <table className="min-w-[800px] w-full text-sm">
              <thead className="bg-gray-50">
                <tr>
                  {LINKED_COLUMNS.map((h) => (
                    <th key={h} className="px-3 py-2 text-left font-medium">
                      {h}
                    </th>
                  ))}
                </tr>
              </thead>
              <tbody>
                {edit.linked.map((lk, index) => {
                  const rowKey = `${lk.uuid || "no-uuid"}-${lk.label}-${index}`;
                  const reason = normalizeReason(lk.reason, index + 3);
                  const reasonText =
                    reason?.type === "rule" ? reason.ruleName || reason.text : reason?.text;
                  return (
                    <tr key={rowKey} className="border-t">
                      <td className="px-3 py-2">
                        <div className="flex flex-col gap-1">
                          <button
                            type="button"
                            className="text-blue-700 underline"
                            onClick={() => {
                              if (typeof onOpenLabel === "function") {
                                onOpenLabel(lk.tenant, lk.label);
                              }
                            }}
                          >
                            {lk.label}
                          </button>
                          {lk.uuid && (
                            <span className="text-xs font-mono text-slate-500">{lk.uuid}</span>
                          )}
                        </div>
                      </td>
                      <td className="px-3 py-2">{lk.tenant}</td>
                      <td className="px-3 py-2">{lk.labelScore ?? "–"}</td>
                      <td className="px-3 py-2">
                        {lk.marks?.length ? lk.marks.map((m, i) => <Tag key={`${rowKey}-${m}-${i}`}>{m}</Tag>) : "–"}
                      </td>
                      <td className="px-3 py-2">{normalizeChangeBy(lk.changeBy)}</td>
                      <td className="px-3 py-2">
                        {reason?.type === "rule" && reason.ruleId ? (
                          <button
                            type="button"
                            className="text-blue-700 underline"
                            onClick={() => {
                              if (typeof onOpenRule === "function") {
                                onOpenRule(reason.ruleId);
                              }
                            }}
                          >
                            {reasonText}
                          </button>
                        ) : (
                          reasonText || "–"
                        )}
                      </td>
                      <td className="px-3 py-2">
                        {canDelete && mode === "edit" && (
                          <button className="rounded-xl border px-2 py-1" onClick={() => unlink(lk.label)}>
                            Unlink
                          </button>
                        )}
                      </td>
                    </tr>
                  );
                })}
              </tbody>
            </table>
          </div>
        ) : (
          <div className="text-sm text-gray-500">–</div>
        )}
      </div>

      {mode === "edit" && canEdit && (
        <div className="mt-4 flex items-center justify-end gap-2">
          <button
            className="rounded-xl border px-4 py-2"
            onClick={() => {
              setEdit({ ...user });
              setMode("view");
              setAdjustmentInput(String(user.adjustments ?? 0));
            }}
          >
            Cancel
          </button>
          <button className="rounded-xl bg-black px-4 py-2 text-white" onClick={save}>
            Save
          </button>
        </div>
      )}

      <div className="mt-6">
        <h4 className="mb-2 text-base font-semibold">User Change History</h4>
        <div className="overflow-auto rounded-xl border">
          <table className="min-w-[900px] w-full text-sm">
            <thead className="bg-gray-50">
              <tr>
                {HISTORY_COLUMNS.map((h) => (
                  <th key={h} className="px-3 py-2 text-left font-medium">
                    {h}
                  </th>
                ))}
              </tr>
            </thead>
            <tbody>
              {(edit.history || []).map((h, i) => {
                const detailText = h.details?.type === "rule" ? h.details.ruleName || h.details.text : h.details?.text;
                return (
                  <tr key={`${edit.uuid}-${i}-${h.when}`} className="border-t">
                    <td className="px-3 py-2">{new Date(h.when).toLocaleString()}</td>
                    <td className="px-3 py-2">{h.event}</td>
                    <td className="px-3 py-2">
                      {h.details?.type === "rule" && h.details.ruleId ? (
                        <button
                          type="button"
                          className="text-blue-700 underline"
                          onClick={() => {
                            if (typeof onOpenRule === "function") {
                              onOpenRule(h.details.ruleId);
                            }
                          }}
                        >
                          {detailText || "View rule"}
                        </button>
                      ) : (
                        detailText || "–"
                      )}
                    </td>
                    <td className="px-3 py-2">{h.previousValue ?? "–"}</td>
                    <td className="px-3 py-2">{h.newValue ?? "–"}</td>
                  </tr>
                );
              })}
              {!(edit.history || []).length && (
                <tr>
                  <td colSpan={HISTORY_COLUMNS.length} className="px-3 py-4 text-center text-sm text-gray-500">
                    No history entries yet.
                  </td>
                </tr>
              )}
            </tbody>
          </table>
        </div>
      </div>

      <Toast text={toast} onHide={() => setToast("")} />

      <AddLabelModal
        open={canEdit && showAdd}
        onClose={() => setShowAdd(false)}
        onSave={(data) => {
          setShowAdd(false);
          MockAPI.attachLabelToUUID(edit.uuid, data).then(({ ok, target }) => {
            if (ok) {
              setEdit({ ...target });
              onChange?.(target);
              showToast("Label attached.");
            }
          });
        }}
        defaultTenant={edit.tenant}
      />
    </div>
  );
}

export default function UserDataPage({ onOpenRule }) {
  const [permissionLevel, setPermissionLevel] = useState(DEFAULT_PERMISSION_LEVEL);
  const perms = useMemo(() => derivePermissions(permissionLevel), [permissionLevel]);
  const [viewMode, setViewMode] = useState("list");
  const [activeUser, setActiveUser] = useState(null);
  const [userLoading, setUserLoading] = useState(false);
  const [refreshSignal, setRefreshSignal] = useState(0);
  const [detailNotice, setDetailNotice] = useState("");
  const pendingUserRequest = React.useRef(0);

  React.useEffect(() => {
    try {
      document.documentElement.classList.remove("dark");
      document.documentElement.style.colorScheme = "light";
      document.body.classList.add("bg-white", "text-black");
    } catch (error) {
      console.warn("Failed to enforce light theme", error);
    }
  }, []);

  const handleBackToList = React.useCallback(() => {
    pendingUserRequest.current += 1;
    setViewMode("list");
    setActiveUser(null);
    setDetailNotice("");
    setUserLoading(false);
  }, []);

  const handleDetailNotFound = React.useCallback(() => {
    setDetailNotice("User not found.");
  }, []);

  const handleOpenUser = React.useCallback(
    (loader, callbacks = {}) => {
      if (!loader) {
        callbacks.onNotFound?.();
        setViewMode("detail");
        setActiveUser(null);
        handleDetailNotFound();
        setUserLoading(false);
        return;
      }
      const requestId = pendingUserRequest.current + 1;
      pendingUserRequest.current = requestId;
      setViewMode("detail");
      setUserLoading(true);
      setDetailNotice("");
      setActiveUser(null);

      Promise.resolve(loader)
        .then((user) => {
          if (pendingUserRequest.current !== requestId) return;
          if (!user) {
            throw new Error("not_found");
          }
          setActiveUser(user);
        })
        .catch(() => {
          if (pendingUserRequest.current !== requestId) return;
          callbacks.onNotFound?.();
          setActiveUser(null);
          handleDetailNotFound();
        })
        .finally(() => {
          if (pendingUserRequest.current === requestId) {
            setUserLoading(false);
          }
        });
    },
    [handleDetailNotFound]
  );

  const handleUserChange = React.useCallback((updated) => {
    setActiveUser(updated);
    setRefreshSignal((token) => token + 1);
  }, []);

  const openByLabel = React.useCallback(
    (tenant, label) => handleOpenUser(MockAPI.findUserByLabel(tenant, label), { onNotFound: handleDetailNotFound }),
    [handleDetailNotFound, handleOpenUser]
  );

  return (
    <div className="min-h-screen bg-white text-black">
      <div className="mx-auto max-w-6xl space-y-6 p-6">
        <header className="space-y-2">
          <h1 className="text-2xl font-semibold">User Data</h1>
        </header>

        <div>
          <label className="block text-xs font-semibold uppercase text-gray-500">Permission Level</label>
          <select
            className="mt-1 w-full max-w-xs rounded-xl border px-3 py-2"
            value={permissionLevel}
            onChange={(e) => setPermissionLevel(e.target.value || DEFAULT_PERMISSION_LEVEL)}
          >
            {PERMISSION_OPTIONS.map((option) => (
              <option key={option} value={option}>
                {option}
              </option>
            ))}
          </select>
          <p className="mt-1 text-xs text-gray-500">
            Read grants view-only access, Write additionally enables refreshing/exporting the list and editing user cards, and
            Delete also allows unlinking labels.
          </p>
        </div>

        <div>
          {viewMode === "list" ? (
            <ListView
              perms={perms}
              onOpenRule={onOpenRule}
              onOpenUser={handleOpenUser}
              refreshSignal={refreshSignal}
            />
          ) : (
            <div className="space-y-4">
              <div className="flex items-center justify-between gap-3">
                <button
                  type="button"
                  className="inline-flex items-center gap-2 rounded-xl border px-3 py-2 text-sm text-slate-700 hover:bg-slate-100"
                  onClick={handleBackToList}
                >
                  ← Back to list
                </button>
                {detailNotice && (
                  <div className="rounded-xl bg-rose-50 px-3 py-2 text-sm text-rose-600">
                    {detailNotice}
                  </div>
                )}
              </div>
              {userLoading ? (
                <div className="rounded-2xl border px-4 py-12 text-center text-sm text-gray-500">
                  Loading user…
                </div>
              ) : activeUser ? (
                <UserCard
                  user={activeUser}
                  perms={perms}
                  onChange={handleUserChange}
                  onOpenLabel={openByLabel}
                  onOpenRule={onOpenRule}
                />
              ) : (
                <div className="rounded-2xl border px-4 py-12 text-center text-sm text-gray-500">
                  User not found.
                </div>
              )}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

function cryptoRandomUUID() {
  if (typeof crypto !== "undefined" && crypto?.randomUUID) return crypto.randomUUID();
  let timestamp = Date.now();
  if (typeof performance !== "undefined" && typeof performance.now === "function") {
    timestamp += performance.now();
  }
  return "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx".replace(/[xy]/g, (c) => {
    const r = (timestamp + Math.random() * 16) % 16 | 0;
    timestamp = Math.floor(timestamp / 16);
    const v = c === "x" ? r : (r & 0x3) | 0x8;
    return v.toString(16);
  });
}
