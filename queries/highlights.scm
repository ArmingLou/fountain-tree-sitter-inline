;; fountain-inline 高亮定义
;; 只包含 inline_note / inline_boneyard / paren_text / text

;; 笔记 [[...]] — 注释色
(inline_note) @comment

;; 注释 /*...*/ — 注释色
(boneyard) @comment

;; 括号 (text) — 类型色，和对话文本区分
(paren_text) @type

;; 普通文本 — 对话色，和action的@text区分
(text) @string
