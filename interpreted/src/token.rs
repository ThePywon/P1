use std::{
  fmt::{Debug, Display},
  str,
};

#[derive(Clone, Copy)]
pub struct Span<'src> {
  source: &'src [u8],
  index: usize,
  length: usize,
  line: usize,
}
impl<'src> Span<'src> {
  pub fn new(source: &'src [u8], index: usize, length: usize, line: usize) -> Self {
    Span {
      source,
      index,
      length,
      line,
    }
  }

  pub fn from(start: &Span<'src>, end: &Span<'src>) -> Self {
    Span {
      source: start.source,
      index: start.index,
      length: end.index - start.index + end.length,
      line: start.line,
    }
  }

  pub fn get_log(&self, filepath: &str, message: &str, lines: u8, warning: bool) -> String {
    assert!(
      lines < 255,
      "Cannot display more than 254 additionnal context lines!"
    );

    let digit_count = (lines as usize + self.line).checked_ilog10().unwrap_or(0) + 1;

    let mut line_offset = 0;
    let mut line_count: u8 = 0;
    let top = &self.source[..self.index];
    let mut start_index = 0;
    for (idx, ch) in top.iter().rev().enumerate() {
      let global_idx = self.index - idx;
      if ch == &b'\n' {
        if line_count == 0 {
          line_offset = idx;
        }

        line_count += 1;
      }

      if line_count > lines {
        start_index = global_idx;
        break;
      }
    }

    line_count = 0;
    let bottom = &self.source[self.index..];
    let mut end_index = self.source.len();
    let mut line_end = self.index + self.length;
    for (idx, ch) in bottom.iter().enumerate() {
      let global_idx = self.index + idx;
      if ch == &b'\n' {
        line_count += 1;
      }

      if line_count == 1 && line_end == self.index + self.length {
        line_end = global_idx;
      }
      if line_count > lines {
        end_index = global_idx;
        break;
      }
    }

    let mut result = format!(
      "\x1b[1m{}\x1b[97m: {}\x1b[0m\n",
      if warning {
        "\x1b[93mwarning"
      } else {
        "\x1b[91merror"
      },
      message
    );

    result.push_str(
      format!(
        "\x1b[96m{}--> \x1b[0m{}:{}:{}\n",
        " ".repeat(digit_count as usize),
        filepath,
        self.line,
        line_offset
      )
      .as_str(),
    );
    result.push_str(
      if start_index == 0 {
        format!(
          "\x1b[96m{} | \x1b[0;2m<----- START OF FILE ----->\x1b[0m",
          " ".repeat(digit_count as usize)
        )
      } else {
        format!("\x1b[96m{} |\x1b[0m", " ".repeat(digit_count as usize))
      }
      .as_str(),
    );
    for (idx, line) in self.source[start_index..end_index]
      .split(|c| c == &b'\n')
      .enumerate()
    {
      let line_idx = idx + self.line - lines as usize;

      result.push_str(format!("\n\x1b[96m{} |\x1b[0m ", line_idx).as_str());
      result.push_str(str::from_utf8(line).unwrap());

      if line_idx == self.line {
        result.push_str(
          format!(
            "\n\x1b[96m{} | {}{}{}",
            " ".repeat(digit_count as usize),
            " ".repeat(line_offset),
            if warning { "\x1b[93m" } else { "\x1b[91m" },
            "^".repeat(self.length)
          )
          .as_str(),
        );
      }
    }
    if end_index == self.source.len() {
      result.push_str(
        format!(
          "\n\x1b[96m{} | \x1b[0;2m<----- END OF FILE ----->\x1b[0m",
          " ".repeat(digit_count as usize)
        )
        .as_str(),
      );
    }

    result
  }

  pub fn get_value(&self) -> &'src str {
    str::from_utf8(&self.source[self.index..(self.index + self.length)]).unwrap()
  }

  pub fn end(&self) -> usize {
    self.index + self.length
  }
}
impl Debug for Span<'_> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    f.debug_struct("Span")
      .field("index", &self.index)
      .field("length", &self.length)
      .field("value", &self.get_value())
      .field("line", &self.line)
      .finish()
  }
}
impl Display for Span<'_> {
  fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
    f.write_str(&self.get_log("main.rs", "test message yippe", 1, true))
  }
}

pub trait TokenKind {
  fn get_meta(&self) -> &str;
}

#[derive(Debug)]
pub struct Token<'src, K: TokenKind> {
  pub kind: K,
  pub span: Span<'src>,
}

#[derive(Debug)]
pub struct SpanContainer<'src, T> {
  pub span: Span<'src>,
  pub item: T,
}
impl<'src, T> SpanContainer<'src, T> {
  pub fn from_token<K: TokenKind>(token: Token<'src, K>, item: T) -> Self {
    Self {
      span: token.span,
      item,
    }
  }

  pub fn boxed(self) -> SpanContainer<'src, Box<T>> {
    SpanContainer {
      span: self.span,
      item: Box::new(self.item),
    }
  }

  pub fn map<O>(self, f: fn(T) -> O) -> SpanContainer<'src, O> {
    SpanContainer {
      span: self.span,
      item: f(self.item),
    }
  }
}
