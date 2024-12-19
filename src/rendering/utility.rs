pub(super) fn create_whitespace_cstring_with_len(len: usize) -> std::ffi::CString {
  let mut buffer: Vec<u8> = Vec::with_capacity(len + 1);
  buffer.extend([b' '].iter().cycle().take(len));
  unsafe { std::ffi::CString::from_vec_unchecked(buffer) }
}
