# 🛠️ System-Level Programming Portfolio

本專案為一份系統導向的程式設計與編譯理論實作集合，涵蓋從資料結構、組譯器設計，到簡易編譯器的完整流程。適合用於展示系統程式設計能力與語言處理技術。

---

## 📁 專案結構
System-Level Programming Portfolio/
 ├── README.md
 ├── 資料結構/
 │   └── Maze、queue、balance tree、hash、graph...
 ├── 程式設計/
 │   ├── sic_assembler
 │   └── compiler_frontend
 └── 程式語言/
     └── PL_Project.cpp、程式設計說明.txt

---

## 🧩 專案模組介紹

### 📚 資料結構實作
- 常見資料結構：Linked List、Binary Search Tree、Stack、Queue
- 實作方式以 C/C++ 為主
- 著重在操作邏輯與記憶體管理練習

### 🧠 SIC Assembler
- 模擬 SIC 指令集架構的組譯器
- 支援符號表管理、轉換成機器碼格式
- 熟悉系統層級語言處理

### ⚙️ Compiler Frontend（語法分析器）
- 模擬 Mini-Compiler 前端（手刻 Parser）
- 處理關鍵字辨識、語法結構分類與語意檢查
- 生成中間碼（Intermediate Representation）

### 🧬 Our-C 簡易編譯器
- 以 C/C++ 為基礎自建一套簡易 C-like 語言編譯器
- 包含詞彙分析（Lexer）、語法分析（Parser）與基本 Codegen
- 探索語言設計與語法規則實作

---

## 🗂️ Git 版本記錄（Tag 說明）

| Tag     | 說明                               |
|---------|------------------------------------|
| v1.0    | 初始版本：資料結構模組完成         |
| v2.0    | 加入 SIC Assembler 模組             |
| v3.0    | 完成 compiler frontend（中間碼產生） |
| v4.0    | Our-C 簡易編譯器加入                |

---

## 🎯 技術總結

- 熟悉 C/C++ 系統程式設計
- 熟練語法分析、語意分析及符號表管理
- 能以手刻實作語言處理工具
- 對編譯器理論與架構有完整實作經驗
