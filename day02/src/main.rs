use lazy_static::lazy_static;
use std::collections::HashMap;
use std::env;
use std::{
    fs::File,
    io::{prelude::*, BufReader},
};

type option = String;
type filename = String;

lazy_static! {
    static ref CUBES_IN_BAG: HashMap<&'static str, u32> =
        HashMap::from([("red", 12), ("green", 13), ("blue", 14)]);
}

fn main() {
    let (option, filepath) = get_args();
    let mut inputs = get_inputs(&filepath);

    match option.as_str() {
        "-p1" => print!("result: {}", { calc_inputs1(&mut inputs) }),
        "-p2" => print!("result: {}", { calc_inputs2(&mut inputs) }),
        _ => panic!("Bad args!"),
    }
}

fn calc_inputs2(inputs: &mut Vec<String>) -> u32 {
    let mut result = 0;
    for line in inputs.iter_mut() {
        result += get_set_power(line);
    }
    result
}

fn get_set_power(line: &mut str) -> u32 {
    let (_id, cubes) = parse_bag(line);

    let (mut fred, mut fgreen, mut fblue) = (0, 0, 0);
    for (color, value) in cubes.iter() {
        if color == &"red" {
            if *value > fred {
                fred = *value;
            }
        } else if *color == "green" {
            if *value > fgreen {
                fgreen = *value;
            }
        } else if *color == "blue" {
            if *value > fblue {
                fblue = *value;
            }
        }
    }

    return fred * fgreen * fblue;
}

fn calc_inputs1(inputs: &mut Vec<String>) -> u32 {
    inputs.iter().map(|line| get_id(line.clone().as_mut_str())).sum()
}

fn get_id(line: &mut str) -> u32 {
    let (id, cubes) = parse_bag(line);

    let all_correct = cubes.iter().all(|(color, value)| {
        CUBES_IN_BAG
            .get(color)
            .map_or(true, |bag_value| *value <= *bag_value)
    });

    if all_correct {
        return id;
    }
    0
}

fn parse_bag(line: &mut str) -> (u32, Vec<(&str, u32)>) {
    let (id, line_) = line.split_once(": ").unwrap();

    let id_ = id[5..id.len()].trim().parse::<u32>().unwrap();

    let cubes: Vec<(&str, u32)> = line_
        .split("; ")
        .flat_map(|l| {
            l.split(';').flat_map(|set| {
                set.split(',').map(|pair| {
                    let mut parts = pair.trim().splitn(2, ' ');
                    if let (Some(quantity_str), Some(color)) = (parts.next(), parts.next()) {
                        if let Ok(quantity) = quantity_str.parse::<u32>() {
                            return Some((color, quantity));
                        }
                    }
                    None
                })
            })
        })
        .filter_map(|x| x)
        .collect();

    (id_, cubes)
}

fn get_inputs(path: &str) -> Vec<String> {
    let buffer = BufReader::new(File::open(path).expect("Failed to open file"));
    buffer.lines().map(|l| l.unwrap()).collect()
}

fn get_args() -> (option, filename) {
    let args: Vec<String> = env::args().collect();
    if args.len() == 3 {
        return (args[1].clone(), args[2].clone());
    }
    panic!("Too few args!");
}
