use lazy_static::lazy_static;
use std::collections::HashMap;
use std::env;
use std::{
    fs::File,
    io::{prelude::*, BufReader},
};

lazy_static! {
    static ref CUBES_IN_BAG: Vec<(&'static str, u32)> =
        vec![("red", 12), ("green", 13), ("blue", 14),];
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
    let (id, line_) = line.split_once(": ").unwrap();
    let sets: Vec<&str> = line_.split("; ").collect();
    let cubes: Vec<(u32, &str)> = sets
        .iter()
        .flat_map(|&l| {
            l.split(';').flat_map(|set| {
                set.split(',').map(|pair| {
                    let mut parts = pair.trim().splitn(2, ' ');
                    let quantity = parts.next().unwrap().parse::<u32>().unwrap();
                    let color = parts.next().unwrap();
                    (quantity, color)
                })
            })
        })
        .collect();

    if cubes.iter().any(|&(quantity, color)| {
        let matching_color = CUBES_IN_BAG
            .iter()
            .find(|&&(bag_color, _)| bag_color == color);
        matching_color.map_or(false, |&(_, bag_quantity)| quantity > bag_quantity)
    }) {
        let id_ = id.get(5..id.len()).unwrap().trim().parse::<u32>().unwrap();
        return id_;
    }
    0
}

fn get_inputs(path: &str) -> Vec<String> {
    let file = File::open(path).expect("Failed to open file");
    let buffer = BufReader::new(file);
    buffer.lines().map(move |l| l.unwrap()).collect()
}

fn get_args() -> String {
    let args: Vec<String> = env::args().collect();
    if args.len() == 2 {
        return args[1].clone();
    }
    panic!("Too few args!");
}
