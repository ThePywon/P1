use super::token::WonTokenKind;

use crate::{token::{Span, Token}, lexer::Lexer};

// eliuablaeaevbo_iueblibc_oiabe+ aibecpaeicbapiec_paeibcpabe_oaiubecoaebic
// test++;

pub struct WonLexer;
impl Lexer<WonTokenKind> for WonLexer {
  fn lex<'input>(input: &'input [u8]) -> Vec<Token<'input, WonTokenKind>> {
    let mut index = 0usize;
    let mut tokens = Vec::new();
    while index < input.len() {
      tokens.push(match input[index] {
        b' ' | b'\t' | b'\n' | b'\r' => { index += 1; continue },
        b'(' => Self::create_token(WonTokenKind::GroupStart, input, &mut index, 1),
        b')' => Self::create_token(WonTokenKind::GroupEnd, input, &mut index, 1),
        b':' => Self::create_token(WonTokenKind::DeclType, input, &mut index, 1),
        b';' => Self::create_token(WonTokenKind::EndStatement, input, &mut index, 1),
        b'=' => Self::create_token(WonTokenKind::Assign, input, &mut index, 1),
        b',' => Self::create_token(WonTokenKind::ItemSeparator, input, &mut index, 1),
        b'a'..=b'z' | b'A'..=b'Z' | b'_' => {
          Self::handle_identifier(&input, &mut index)
        },
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

          Self::create_token(WonTokenKind::Number, input, &mut index, offset)
        }
        unhandled => {
          let ch = unhandled as char;
          if ch.is_control() { index += 1; }
          else { panic!("Unexpected char: {:?}", ch) }
          continue
        }
      });
    }

    tokens
  }
}
impl WonLexer {
  fn handle_identifier<'input>(input: &'input [u8], index: &mut usize) -> Token<'input, WonTokenKind> {
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
      _ => WonTokenKind::Identifier
    };

    Self::create_token(kind, input, index, identifier.len())
  }

  fn create_token<'input>(kind: WonTokenKind, input: &'input [u8], index: &mut usize, length: usize) -> Token<'input, WonTokenKind> {
    let span = Span { source: input, index: *index, length };
    *index += length;
    Token { kind, span }
  }
}
