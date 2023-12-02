use lazy_static::lazy_static;
use std::collections::HashMap;
use std::env;
use std::{
    fs::File,
    io::{prelude::*, BufReader},
};

lazy_static! {
    static ref CUBES_IN_BAG: HashMap<&'static str, u32> =
        HashMap::from([("red", 12), ("green", 13), ("blue", 14)]);
}

fn main() {
    let filepath = get_args();
    let mut inputs = get_inputs(&filepath);
    let result = calc_inputs(&mut inputs);
    print!("result: {}", { result });
}

fn calc_inputs(inputs: &mut Vec<String>) -> u32 {
    let mut result = 0;
    for line in inputs.iter_mut() {
        result += get_id(line);
    }
    return result;
}

fn get_id(line: &mut str) -> u32 {
    let mut cubes = HashMap::from([("red", 0), ("green", 0), ("blue", 0)]);
    for cube in cubes.iter_mut() {
        if let Some(pos) = line.find(cube.0) {
            if pos > 0 {
                if let Some(digit) = line.chars().nth(pos - 1) {
                    *cube.1 += digit.to_digit(10).unwrap();
                }
            }
        }
    }

    if cubes
        .iter()
        .all(|(color, count)| *count <= *CUBES_IN_BAG.get(color).unwrap_or(&0))
    {
        if let Some(mut id_pos) = line.find("Game") {
            id_pos += 4;
            if let Some(id) = line.chars().nth(id_pos) {
                return id as u32;
            }
        }
    }
    0
}

fn get_inputs(path: &str) -> Vec<String> {
    let file = File::open(path).expect("Failed to open file");
    let buffer = BufReader::new(file);
    buffer
        .lines()
        .map(move |l| {
            l.expect("Could not parse line")
                .replace(char::is_whitespace, "")
        })
        .collect()
}

fn get_args() -> String {
    let args: Vec<String> = env::args().collect();
    if args.len() == 2 {
        return args[1].clone();
    }
    panic!("Too few args!");
}
