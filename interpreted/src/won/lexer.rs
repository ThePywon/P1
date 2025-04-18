use super::token::WonTokenKind;

use crate::{
  lexer::Lexer,
  token::{Span, Token},
};

// eliuablaeaevbo_iueblibc_oiabe+ aibecpaeicbapiec_paeibcpabe_oaiubecoaebic
// test++;

pub struct WonLexer;
impl Lexer<WonTokenKind> for WonLexer {
  fn lex(input: &[u8]) -> Vec<Token<'_, WonTokenKind>> {
    let mut index = 0usize;
    let mut line_count = 1usize;
    let mut tokens = Vec::new();
    while index < input.len() {
      tokens.push(match input[index] {
        b' ' | b'\t' | b'\r' => {
          index += 1;
          continue;
        }
        b'\n' => {
          index += 1;
          line_count += 1;
          continue;
        }
        b'(' => Self::create_token(WonTokenKind::GroupStart, input, &mut index, 1, line_count),
        b')' => Self::create_token(WonTokenKind::GroupEnd, input, &mut index, 1, line_count),
        b':' => Self::create_token(WonTokenKind::DeclType, input, &mut index, 1, line_count),
        b';' => Self::create_token(WonTokenKind::EndStatement, input, &mut index, 1, line_count),
        b'=' => Self::create_token(WonTokenKind::Assign, input, &mut index, 1, line_count),
        b',' => Self::create_token(
          WonTokenKind::ItemSeparator,
          input,
          &mut index,
          1,
          line_count,
        ),
        b'a'..=b'z' | b'A'..=b'Z' | b'_' => Self::handle_identifier(input, &mut index, line_count),
        b'0'..=b'9' => {
          let mut offset = 1;
          let mut peek = input.get(index + offset).copied();
          while peek.is_some() {
            if peek.unwrap().is_ascii_digit() || peek.unwrap() == b'_' {
              offset += 1;
              peek = input.get(index + offset).copied();
              continue;
            }
            break;
          }

          Self::create_token(WonTokenKind::Number, input, &mut index, offset, line_count)
        }
        unhandled => {
          let ch = unhandled as char;
          if ch.is_control() {
            index += 1;
          } else {
            panic!("Unexpected char: {:?}", ch)
          }
          continue;
        }
      });
    }

    tokens
  }
}
impl WonLexer {
  fn handle_identifier<'input>(
    input: &'input [u8],
    index: &mut usize,
    line: usize,
  ) -> Token<'input, WonTokenKind> {
    let remainder = &input[*index..];
    let mut end = 0;
    for (idx, ch) in remainder.iter().enumerate() {
      if ch.is_ascii_alphanumeric() || *ch == b'_' {
        end = idx + 1;
        continue;
      }
      break;
    }
    let identifier = &remainder[..end];
    let kind = match identifier {
      b"let" => WonTokenKind::Let,
      b"const" => WonTokenKind::Const,
      b"as" => WonTokenKind::As,
      b"false" => WonTokenKind::False,
      b"true" => WonTokenKind::True,
      _ => WonTokenKind::Identifier,
    };

    Self::create_token(kind, input, index, identifier.len(), line)
  }

  fn create_token<'input>(
    kind: WonTokenKind,
    input: &'input [u8],
    index: &mut usize,
    length: usize,
    line: usize,
  ) -> Token<'input, WonTokenKind> {
    let span = Span::new(input, *index, length, line);
    *index += length;
    Token { kind, span }
  }
}
