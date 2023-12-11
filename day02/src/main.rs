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
    let (option, filepath) = get_args();
    let mut inputs = get_inputs(&filepath);
    match option.as_str() {
        "-p1" => {
            let result = calc_inputs1(&mut inputs);
            print!("result: {}", { result });
        }
        "-p2" => {
            let result = calc_inputs2(&mut inputs);
            print!("result: {}", { result });
        }
        _ => {
            panic!("Bad args!");
        }
    }
}

fn calc_inputs2(inputs: &mut Vec<String>) -> u32 {
    let mut result = 0;
    for line in inputs.iter_mut() {
        result += get_set_power(line);
    }
    return result;
}

fn get_set_power(line: &mut str) -> u32 {
    let (_id, cubes) = parse_bag(line);

    let (mut fred, mut fgreen, mut fblue) = (0, 0, 0);
    for (color, value) in cubes.iter() {
        if color == &"red" {
            if *value > fred {
                fred = *value;
            }
        } else if color == &"green" {
            if *value > fgreen {
                fgreen = *value;
            }
        } else if color == &"blue" {
            if *value > fblue {
                fblue = *value;
            }
        }
    }

    return fred * fgreen * fblue;
}

fn calc_inputs1(inputs: &mut Vec<String>) -> u32 {
    let mut result = 0;
    for line in inputs.iter_mut() {
        result += get_id(line);
    }
    return result;
}

fn get_id(line: &mut str) -> u32 {
    let (id, cubes) = parse_bag(line);

    let mut all_correct = true;
    for (color, value) in cubes.iter() {
        if let Some(&bag_value) = CUBES_IN_BAG.get(color) {
            all_correct &= *value <= bag_value;
        }
    }

    if all_correct {
        id
    } else {
        0
    }
}

fn parse_bag(line: &mut str) -> (u32, Vec<(&str, u32)>) {
    let (id, line_) = line.split_once(": ").unwrap();

    let id_ = id.get(5..id.len()).unwrap().trim().parse::<u32>().unwrap();
    let sets: Vec<&str> = line_.split("; ").collect();
    let cubes: Vec<(&str, u32)> = sets
        .iter()
        .flat_map(|&l| {
            l.split(';').flat_map(|set| {
                set.split(',').map(|pair| {
                    let mut parts = pair.trim().splitn(2, ' ');
                    let quantity = parts.next().unwrap().parse::<u32>().unwrap();
                    let color = parts.next().unwrap();
                    (color, quantity)
                })
            })
        })
        .collect();

    return (id_, cubes);
}

fn get_inputs(path: &str) -> Vec<String> {
    let file = File::open(path).expect("Failed to open file");
    let buffer = BufReader::new(file);
    buffer.lines().map(move |l| l.unwrap()).collect()
}

fn get_args() -> (String, String) {
    let args: Vec<String> = env::args().collect();
    if args.len() == 3 {
        return (args[1].clone(), args[2].clone());
    }
    panic!("Too few args!");
}