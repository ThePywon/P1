use thiserror::Error;

#[derive(Error, Debug)]
pub enum SystemError {
  #[error("Not all query items in system were unique.")]
  QueryDeadlock,
}
