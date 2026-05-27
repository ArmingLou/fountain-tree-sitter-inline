module.exports = grammar({
  name: 'fountain_inline',

  externals: $ => [
    $.inline_note,
    $.inline_boneyard,
  ],

  rules: {
    document: $ => repeat(choice(
      $.inline_note,
      $.inline_boneyard,
      $.paren_text,
      $.text
    )),

    paren_text: $ => /\([^)\n]+\)/,

    text: $ => /[^\[\(/\n]+|\/|\n|./,
  }
});
