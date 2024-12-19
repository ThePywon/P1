#[derive(Debug)]
pub enum ComponentError {
  Unregistered,
  AlreadyExistsForEntity,
  NotFoundForEntity,
  // SHOULD NEVER GET THAT ERROR
  // This error occurs when I somehow don't catch an unregistered component
  // And end up with a component id that doesn't exist
  InvalidId,
  NotEnoughAllocatedSpace
}

#[derive(Debug)]
pub enum EntityError {
  NotFound,
  // SHOULD NEVER GET THAT ERROR
  MismatchedComponentBitmask
}

#[derive(Debug)]
pub enum P1Error {
  Component(ComponentError),
  Entity(EntityError)
}

impl From<ComponentError> for P1Error {
  fn from(value: ComponentError) -> Self {
    P1Error::Component(value)
  }
}
impl From<EntityError> for P1Error {
  fn from(value: EntityError) -> Self {
    P1Error::Entity(value)
  }
}
