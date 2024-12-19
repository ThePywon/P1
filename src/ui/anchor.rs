use glfw::Window;

// Might wanna massively rework this once I revisit it

pub struct Anchor<'a> {
  win: &'a Window,
  pos: (f32, f32)
}

pub enum AnchorPosition {
  TopLeft,
  Top,
  TopRight,
  Left,
  Center,
  Right,
  BottomLeft,
  Bottom,
  BottomRight
}

impl<'a> Anchor<'a> {
  pub fn new(win: &'a Window, pos: AnchorPosition) -> Self {
    let (x, y);

    match pos {
      AnchorPosition::TopLeft => {
        (x, y) = (0.0, 0.0)
      }
      AnchorPosition::Top => {
        (x, y) = (0.5, 0.0)
      }
      AnchorPosition::TopRight => {
        (x, y) = (1.0, 0.0)
      }
      AnchorPosition::Left => {
        (x, y) = (0.0, 0.5)
      }
      AnchorPosition::Center => {
        (x, y) = (0.5, 0.5)
      }
      AnchorPosition::Right => {
        (x, y) = (1.0, 0.5)
      }
      AnchorPosition::BottomLeft => {
        (x, y) = (0.0, 1.0)
      }
      AnchorPosition::Bottom => {
        (x, y) = (0.5, 1.0)
      }
      AnchorPosition::BottomRight => {
        (x, y) = (1.0, 1.0)
      }
    }

    Anchor { win, pos: (x, y) }
  }

  pub fn new_custom(win: &'a Window, tx: f32, ty: f32) -> Self {
    let (mut x, mut y) = (tx, ty);

    if x < 0.0 { x = 0.0; }
    else if x > 1.0 { x = 1.0; }
    if y < 0.0 { y = 0.0; }
    else if y > 1.0 { y = 1.0; }

    Anchor { win, pos: (x, y) }
  }

  pub fn position(&self) -> (i32, i32) {
    let (x, y) = self.win.get_size();
    (f32::floor(x as f32 * self.pos.0) as i32,
    f32::floor(y as f32 * self.pos.1) as i32)
  }
}
