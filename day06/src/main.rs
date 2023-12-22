use std::collections::HashMap;
use std::env;
use std::fs;

fn main() {
    let (option, path) = get_args();

    let result: u32;
    match option.as_str() {
        "-p1" => {
            result = part1(&read_input(&path));
        }
        "-p2" => {
            result = part2(&read_input(&path));
        }
        _ => {
            panic!("Invalid args!");
        }
    }
    println!("result: {}", { result });
}

fn part2(input: &str) -> u32 {
    0
}

fn part1(input: &str) -> u32 {
    let results = get_race_results(input);

    let mut result = 1u32;
    for (time, distance) in results.clone() {
        result *= ways_of_win(time, distance);
    }

    result
}

fn ways_of_win(time: u32, distance: u32) -> u32 {
    let mut counter = 0u32;
    for n in 1..time {
        let race = (time - n) * n;
        if race > distance {
            counter += 1;
        }
    }

    counter
}

fn get_race_results(input: &str) -> HashMap<u32, u32> {
    let (first_ln, second_ln) = input.split_once('\n').expect("Failure splitting string!");

    let times: Vec<u32> = first_ln
        .split_whitespace()
        .map(|s| s.parse::<u32>())
        .filter_map(Result::ok)
        .collect();

    let distances: Vec<u32> = second_ln
        .split_whitespace()
        .map(|s| s.parse::<u32>())
        .filter_map(Result::ok)
        .collect();

    let result: HashMap<u32, u32> = times.into_iter().zip(distances.into_iter()).collect();

    result
}

fn read_input(path: &str) -> String {
    fs::read_to_string(&path).expect("Failure reading file!")
}

fn get_args() -> (String, String) {
    let args: Vec<String> = env::args().collect();
    if args.len() == 3 {
        return (args[1].clone(), args[2].clone());
    }
    panic!("Too few args!");
}
