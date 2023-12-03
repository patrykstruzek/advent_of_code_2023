// I know that this is not the best solution, among other things, because there is a lot of code repetition,
// but it is what it is. Maybe I'll fix it in the future.
use lazy_static::lazy_static;
use std::env;
use std::{
    fs::File,
    io::{prelude::*, BufReader},
};

lazy_static! {
    static ref CUBES_IN_BAG: Vec<(&'static str, u32)> =
        vec![("red", 12), ("green", 13), ("blue", 14)];
}

fn main() {
    let (filepath, option) = get_args();
    let mut inputs = get_inputs(&filepath);
    if option == "-p1" {
        let result = calc_inputs1(&mut inputs);
        print!("result: {}", { result });
    } else if option == "-p2" {
        let result = calc_inputs2(&mut inputs);
        print!("result: {}", { result });
    } else {
        panic!("Bad args!");
    }
}

fn calc_inputs2(inputs: &mut Vec<String>) -> u32 {
    let mut result = 0;
    for (n, line) in inputs.iter_mut().enumerate() {
        println!("game {}", n + 1);
        result += get_set_power(line);
    }
    return result;
}

fn get_set_power(line: &mut str) -> u32 {
    let (id, line_) = line.split_once(": ").unwrap();
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
    for (n, line) in inputs.iter_mut().enumerate() {
        println!("game {}", n + 1);
        result += get_id(line);
    }
    return result;
}

fn get_id(line: &mut str) -> u32 {
    let (id, line_) = line.split_once(": ").unwrap();
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

    let mut all_correct = true;
    for (color, value) in cubes.iter() {
        println!("color {} quantity {}", color, value);
        if let Some(&bag_value) = CUBES_IN_BAG
            .iter()
            .find(|&&(bag_color, _)| bag_color == *color)
        {
            println!("compare {} {}", bag_value.1, value);
            all_correct &= *value <= bag_value.1;
        } else {
            all_correct = false;
        }
    }

    if all_correct {
        let id_ = id.get(5..id.len()).unwrap().trim().parse::<u32>().unwrap();
        println!("id {}", id_);
        id_
    } else {
        0
    }
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
