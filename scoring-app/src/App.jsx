import React from "react";
import { useState } from "react";
import RulesPage from "./pages/RulesPage.jsx";
import UserDataPage from "./pages/UserDataPage.jsx";

const NAVIGATION = [
  {
    id: "rules",
    title: "Rules",
    description: "Configure and manage scoring logic",
  },
  {
    id: "user-data",
    title: "User Data",
    description: "Review user level scoring context",
  },
];

export default function App() {
  const [active, setActive] = useState(null);
  const [sidebarOpen, setSidebarOpen] = useState(false);
  const [focusedRuleId, setFocusedRuleId] = useState(null);

  const handleSelect = (next) => {
    setActive(next);
    setSidebarOpen(false);
  };

  const handleOpenRule = (ruleId) => {
    if (!ruleId) return;
    setFocusedRuleId(ruleId);
    setActive("rules");
    setSidebarOpen(false);
  };

  const renderActiveView = () => {
    if (!active) return null;
    if (active === "rules") {
      return (
        <RulesPage
          focusRuleId={focusedRuleId}
          onRuleFocusHandled={() => setFocusedRuleId(null)}
        />
      );
    }
    if (active === "user-data") {
      return <UserDataPage onOpenRule={handleOpenRule} />;
    }
    return null;
  };

  return (
    <div className="relative min-h-screen bg-slate-100 text-slate-900">
      <button
        type="button"
        onClick={() => setSidebarOpen((prev) => !prev)}
        className="fixed left-4 top-4 z-50 inline-flex h-10 w-10 items-center justify-center rounded-full border border-slate-300 bg-white text-slate-700 shadow-md transition hover:bg-slate-50 focus:outline-none focus:ring-2 focus:ring-slate-400 focus:ring-offset-2 focus:ring-offset-white"
        aria-label={`${sidebarOpen ? "Close" : "Open"} navigation`}
        aria-expanded={sidebarOpen}
        aria-pressed={sidebarOpen}
      >
        <span className="flex flex-col items-center justify-center gap-1">
          <span className="block h-0.5 w-5 rounded bg-slate-700" />
          <span className="block h-0.5 w-5 rounded bg-slate-700" />
          <span className="block h-0.5 w-5 rounded bg-slate-700" />
        </span>
      </button>
      <div className="flex min-h-screen">
        {sidebarOpen && (
          <aside
            className="flex w-72 flex-col border-r border-slate-200 bg-white pt-16 shadow-sm"
            aria-label="Scoring sections"
          >
            <div className="border-b border-slate-200 px-6 pb-6">
              <h1 className="mt-1 text-2xl font-semibold text-slate-900">Scoring</h1>
            </div>
            <nav className="flex-1 overflow-y-auto p-2">
              <p className="px-4 py-2 text-xs font-semibold uppercase tracking-wide text-slate-500">
                Sections
              </p>
              <ul className="space-y-1">
                {NAVIGATION.map((item) => {
                  const isActive = active === item.id;
                  return (
                    <li key={item.id}>
                      <button
                        type="button"
                        onClick={() => handleSelect(item.id)}
                        className={`w-full rounded-xl px-4 py-3 text-left transition ${
                          isActive
                            ? "bg-slate-900 text-white shadow"
                            : "text-slate-700 hover:bg-slate-100"
                        }`}
                      >
                        <div className="font-semibold">{item.title}</div>
                        <p
                          className={`text-xs ${
                            isActive ? "text-slate-200" : "text-slate-500"
                          }`}
                        >
                          {item.description}
                        </p>
                      </button>
                    </li>
                  );
                })}
              </ul>
            </nav>
          </aside>
        )}
        <main className="flex-1 overflow-y-auto">
          <div className="mx-auto min-h-screen max-w-[1600px]">
            {renderActiveView() || (
              <div className="flex min-h-screen flex-col items-center justify-center px-6 text-center text-slate-500">
                <h2 className="text-3xl font-semibold text-slate-700">Scoring Service</h2>
                <p className="mt-4 max-w-xl text-base">
                  Click the three dots in the sidebar to choose or switch between sections
                </p>
              </div>
            )}
          </div>
        </main>
      </div>
    </div>
  );
}
