/** @type {import('jest').Config} */
module.exports = {
  preset: "ts-jest",
  testEnvironment: "node",
  roots: ["<rootDir>/tests"],
  collectCoverageFrom: ["src/**/*.ts"],
  coverageThreshold: {
    global: { statements: 95, branches: 90, functions: 95, lines: 95 },
  },
};
