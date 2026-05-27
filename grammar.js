module.exports = grammar({
  name: 'fountain_inline',

  rules: {
    document: $ => repeat(choice(
      $.inline_note,
      $.inline_boneyard,
      $.paren_text,
      $.text
    )),

    // 支持中文括号（ ）和英文括号 ( )
    paren_text: $ => /（[^）\n]+）|\([^)\n]+\)/,

    // inline_note: [[...]] 支持跨行
    // 使用正则匹配，兼容嵌套的 [[ ]]
    inline_note: $ => token(/\[\[[^\]]*(?:\][^\]]*)*\]\]/),

    // inline_boneyard: /*...*/ 支持跨行
    inline_boneyard: $ => token(/\/\*[^*]*(?:\*[^\/][^*]*)*\*\//),

    // text: 以上三个都匹配不到时的默认值（最低优先级）
    text: $ => token(prec(-1, /[^\[\(\/\n（）]+|\/|\n|./))
  }
});