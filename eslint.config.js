const js = require("@eslint/js");
const tseslint = require("typescript-eslint");

module.exports = tseslint.config(
  { ignores: ["dist/", "coverage/", "legacy/", "node_modules/", ".venv/", "*.js"] },
  js.configs.recommended,
  ...tseslint.configs.recommended,
);
