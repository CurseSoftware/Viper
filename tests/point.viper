struct Vec2 
{
    i: i32;
    j: i32;

    define new(new_i: i32, new_j: i32) -> Vec2 
    {
        return Vec2 {
            i: new_i,
            j: new_j
        };
    }
}

struct Vec3 
{
    i: i32;
    j: i32;
    k: i32;
}

define add_points(Vec2 a, Vec2 b) -> Point 
{
    return Vec2::new(a.i + b.i, a.j + b.j);
}

define main() -> i32
{
    let my_integer: i32 = 1__000_000;
    let pointA: Vec2 = Vec2::new(1, 2);
    let pointB: Vec2 = Vec2::new(1, 2);
    let pointC: Vec2 = add_points(pointA, pointB);

    return 0;
}
