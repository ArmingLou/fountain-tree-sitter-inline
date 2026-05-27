module.exports = grammar({
  name: 'fountain_inline',

  extras: $ => [/\n/],

  externals: $ => [
    $.paren_text,
    $.boneyard_start,
    $.note_start,
  ],

  rules: {
    document: $ => repeat1($._element),

    _element: $ => choice(
      $.boneyard,
      $.inline_note,
      $.paren_text,
      $.text
    ),

    boneyard: $ => seq(
      $.boneyard_start,
      repeat1(choice(/[^*]+/, /\*[^\/]/)),
      '*/'
    ),

    inline_note: $ => seq(
      $.note_start,
      repeat1(choice(/[^\]]+/, /\][^\]]/)),
      ']]'
    ),

    // text 接受所有字符包括 (, 让 scanner 在行首优先级更高
    text: $ => token(prec(-1, /[^\/\[]/))
  }
});