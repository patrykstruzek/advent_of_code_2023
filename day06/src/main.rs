use std::env;
use std::fs;

type option = String;
type filename = String;

fn main() {
    let (option, path) = get_args();

    match option.as_str() {
        "-p1" => println!("result: {}", { part1(&read_input(&path)) }),
        "-p2" => println!("result: {}", { part2(&read_input(&path)) }),
        _ => panic!("Invalid args!"),
    };
}

fn part2(input: &str) -> u64 {
    let results = get_race_results(input);

    let (time, distance) = results
        .iter()
        .map(|(time, distance)| (*time, *distance))
        .fold((0, 0), |acc, (time, distance)| {
            (acc.0 * 100 + time, acc.1 * 10000 + distance)
        });

    ways_of_win(time, distance)
}

fn part1(input: &str) -> u64 {
    let results = get_race_results(input);

    results
        .iter()
        .map(|&(time, distance)| ways_of_win(time as u64, distance as u64))
        .product()
}

fn ways_of_win(time: u64, distance: u64) -> u64 {
    (1..time).filter(|&n| (time - n) * n > distance).count() as u64
}

fn get_race_results(input: &str) -> Vec<(u64, u64)> {
    let (first_ln, second_ln) = input.split_once('\n').expect("Failure splitting string!");

    let times: Vec<u64> = first_ln
        .split_whitespace()
        .map(|s| s.parse())
        .filter_map(Result::ok)
        .collect();

    let distances: Vec<u64> = second_ln
        .split_whitespace()
        .map(|s| s.parse())
        .filter_map(Result::ok)
        .collect();

    let result: Vec<(u64, u64)> = times
        .into_iter()
        .zip(distances.into_iter())
        .map(|(time, distance)| (u64::from(time), u64::from(distance)))
        .collect();

    result
}

fn read_input(path: &str) -> String {
    fs::read_to_string(&path).expect("Failure reading file!")
}

fn get_args() -> (option, filename) {
    let args: Vec<String> = env::args().collect();
    if args.len() == 3 {
        return (args[1].clone(), args[2].clone());
    }
    panic!("Too few args!");
}
