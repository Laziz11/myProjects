
const now = () => new Date().toISOString();

const makeManualValue = (text) => ({
  mode: "manual",
  previousMode: "manual",
  text,
  list: "",
  substring: text,
});

const makeListValue = (list) => ({
  mode: "list",
  previousMode: "list",
  text: "",
  list,
  substring: "",
});

const makeSubstringValue = (text) => ({
  mode: "substring",
  previousMode: "manual",
  text,
  list: "",
  substring: text,
});

export const RULE_SEEDS = [
  {
    id: "rule-high-deposits",
    name: "High deposits risk",
    description: "Users with > 20 deposits and risk mark",
    resultType: "Score",
    value: 34,
    active: true,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-high-deposits-group-1",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-high-deposits-group-1-condition-1", parameter: "deposits_count", operator: ">", value: 20 },
          { id: "rule-high-deposits-group-1-condition-2", parameter: "deposits_sum", operator: ">", value: 500 },
        ],
        subgroups: [],
      },
      {
        id: "rule-high-deposits-group-2",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-high-deposits-group-2-condition-1", parameter: "risk_mark", operator: "==", value: true },
        ],
        subgroups: [],
      },
    ],
  },
  {
    id: "rule-tenant-allowlist",
    name: "Tenant-specific allowlist",
    description: "Allow VIP tenant users via curated system list",
    resultType: "Mark",
    value: "VIP",
    active: true,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-tenant-allowlist-group-1",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-tenant-allowlist-group-1-condition-1", parameter: "tenant", operator: "==", value: "Crorewin" },
          { id: "rule-tenant-allowlist-group-1-condition-2", parameter: "email", operator: "==", value: makeListValue("vip_allowlist") },
        ],
        subgroups: [
          {
            id: "rule-tenant-allowlist-group-1-sub-1",
            level: 2,
            operator: "OR",
            not: false,
            conditions: [
              { id: "rule-tenant-allowlist-group-1-sub-1-condition-1", parameter: "platform", operator: "==", value: "IOS" },
              { id: "rule-tenant-allowlist-group-1-sub-1-condition-2", parameter: "platform", operator: "==", value: "Android" },
            ],
            subgroups: [],
          },
        ],
      },
    ],
  },
  {
    id: "rule-new-users-low-risk",
    name: "New users – low risk",
    description: "Recent registration without payouts",
    resultType: "Score",
    value: -10,
    active: true,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-new-users-low-risk-group-1",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-new-users-low-risk-group-1-condition-1", parameter: "registration_date", operator: ">", value: "2025-09-01" },
          { id: "rule-new-users-low-risk-group-1-condition-2", parameter: "registration_date", operator: "<", value: "2026-06-01" },
        ],
        subgroups: [
          {
            id: "rule-new-users-low-risk-group-1-sub-1",
            level: 2,
            operator: "AND",
            not: false,
            conditions: [
              { id: "rule-new-users-low-risk-group-1-sub-1-condition-1", parameter: "payouts_count", operator: "==", value: 0 },
              { id: "rule-new-users-low-risk-group-1-sub-1-condition-2", parameter: "deposits_sum", operator: "<", value: 300 },
            ],
            subgroups: [],
          },
        ],
      },
      {
        id: "rule-new-users-low-risk-group-2",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-new-users-low-risk-group-2-condition-1", parameter: "mark_fake", operator: "==", value: false },
        ],
        subgroups: [],
      },
    ],
  },
  {
    id: "rule-payout-velocity-alert",
    name: "Payout velocity alert",
    description: "Frequent high payouts with prior risk mark",
    resultType: "Score",
    value: 26,
    active: true,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-payout-velocity-alert-group-1",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-payout-velocity-alert-group-1-condition-1", parameter: "payouts_sum", operator: ">", value: 5000 },
        ],
        subgroups: [
          {
            id: "rule-payout-velocity-alert-group-1-sub-1",
            level: 2,
            operator: "AND",
            not: false,
            conditions: [
              { id: "rule-payout-velocity-alert-group-1-sub-1-condition-1", parameter: "payouts_count", operator: ">", value: 5 },
              { id: "rule-payout-velocity-alert-group-1-sub-1-condition-2", parameter: "tenant", operator: "==", value: "4Rabet" },
            ],
            subgroups: [],
          },
        ],
      },
      {
        id: "rule-payout-velocity-alert-group-2",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-payout-velocity-alert-group-2-condition-1", parameter: "risk_mark", operator: "==", value: true },
          { id: "rule-payout-velocity-alert-group-2-condition-2", parameter: "multi_account", operator: ">", value: 0 },
        ],
        subgroups: [],
      },
    ],
  },
  {
    id: "rule-duplicate-label-detector",
    name: "Duplicate label detector",
    description: "Merge identities with repeated labels and fraud flags",
    resultType: "Mark",
    value: "Fraud",
    active: true,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-duplicate-label-detector-group-1",
        level: 1,
        operator: "OR",
        not: false,
        conditions: [
          { id: "rule-duplicate-label-detector-group-1-condition-1", parameter: "multi_account", operator: ">", value: 1 },
          { id: "rule-duplicate-label-detector-group-1-condition-2", parameter: "mark_fake", operator: "==", value: true },
        ],
        subgroups: [
          {
            id: "rule-duplicate-label-detector-group-1-sub-1",
            level: 2,
            operator: "AND",
            not: false,
            conditions: [
              { id: "rule-duplicate-label-detector-group-1-sub-1-condition-1", parameter: "email", operator: "==", value: makeListValue("blocked_emails") },
            ],
            subgroups: [
              {
                id: "rule-duplicate-label-detector-group-1-sub-1-sub-1",
                level: 3,
                operator: "AND",
                not: false,
                conditions: [
                  { id: "rule-duplicate-label-detector-group-1-sub-1-sub-1-condition-1", parameter: "fingerprint", operator: "has", value: makeSubstringValue("shadow") },
                ],
                subgroups: [],
              },
            ],
          },
        ],
      },
    ],
  },
  {
    id: "rule-vip-high-roller-track",
    name: "VIP high roller track",
    description: "Surface large positive lifetime value players for VIP routing",
    resultType: "Mark",
    value: "VIP",
    active: false,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-vip-high-roller-track-group-1",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-vip-high-roller-track-group-1-condition-1", parameter: "deposits_sum", operator: ">", value: 20000 },
          { id: "rule-vip-high-roller-track-group-1-condition-2", parameter: "deposits_count", operator: ">", value: 12 },
        ],
        subgroups: [
          {
            id: "rule-vip-high-roller-track-group-1-sub-1",
            level: 2,
            operator: "AND",
            not: false,
            conditions: [
              { id: "rule-vip-high-roller-track-group-1-sub-1-condition-1", parameter: "payouts_sum", operator: "<", value: 5000 },
              { id: "rule-vip-high-roller-track-group-1-sub-1-condition-2", parameter: "tenant", operator: "==", value: "Crorebet" },
            ],
            subgroups: [
              {
                id: "rule-vip-high-roller-track-group-1-sub-1-sub-1",
                level: 3,
                operator: "OR",
                not: false,
                conditions: [
                  { id: "rule-vip-high-roller-track-group-1-sub-1-sub-1-condition-1", parameter: "platform", operator: "==", value: "IOS" },
                  { id: "rule-vip-high-roller-track-group-1-sub-1-sub-1-condition-2", parameter: "platform", operator: "==", value: "Android" },
                ],
                subgroups: [],
              },
            ],
          },
        ],
      },
    ],
  },
  {
    id: "rule-suspicious-fp-substring",
    name: "Suspicious fingerprint substring",
    description: "Flag patterns that match risky device fingerprints",
    resultType: "Score",
    value: 12,
    active: true,
    groupsOperator: "AND",
    groups: [
      {
        id: "rule-suspicious-fp-substring-group-1",
        level: 1,
        operator: "AND",
        not: false,
        conditions: [
          { id: "rule-suspicious-fp-substring-group-1-condition-1", parameter: "fingerprint", operator: "has", value: makeSubstringValue("shadow"), not: true },
          { id: "rule-suspicious-fp-substring-group-1-condition-2", parameter: "tenant", operator: "==", value: "Crorebet" },
        ],
        subgroups: [],
      },
    ],
  },
];

const cloneGroup = (group) => ({
  ...group,
  conditions: (group.conditions || []).map((condition) => ({ ...condition })),
  subgroups: (group.subgroups || []).map(cloneGroup),
});

export function createInitialRules() {
  return RULE_SEEDS.map((rule) => ({
    ...rule,
    createdAt: now(),
    updatedAt: now(),
    groups: rule.groups.map(cloneGroup),
  }));
}

export function getRuleSeed(ruleId) {
  return RULE_SEEDS.find((rule) => rule.id === ruleId) || null;
}

export function getRuleSeedsMap() {
  return RULE_SEEDS.reduce((acc, rule) => {
    acc[rule.id] = rule;
    return acc;
  }, {});
}
