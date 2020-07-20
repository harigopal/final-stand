type t = {
  damage: int,
  position,
}
and position =
  | Above(int)
  | Below(int);
