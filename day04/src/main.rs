use std::env;
use std::fs;

type option = String;
type filename = String;

fn main() {
    let (option, path) = get_args();

    match option.as_str() {
        "-p1" => print!("result: {}", part1(&read_inputs(&path))),
        "-p2" => print!("result: {}", part2(&read_inputs(&path))),
        _ => panic!("Invalid args!"),
    }
}

fn part2(inputs: &str) -> u32 {
    0
}

fn part1(inputs: &str) -> u32 {
    let mut result: u32 = 0;
    for line in inputs.lines() {
        result += parse_line(line.to_owned());
    }
    result
}

fn parse_line(line: String) -> u32 {
    let (wn, sn) = parse_card(&line);

    let mut counter: u32 = 0;
    for n in sn.iter() {
        wn.iter().for_each(|n_| {
            if n_ == n {
                counter += 1;
            }
        });
    }

    if counter == 1 {
        return 1;
    } else if counter > 1 {
        return u32::pow(2, counter - 1);
    }
    0
}

fn parse_card(line: &str) -> (Vec<u32>, Vec<u32>) {
    let (_card, numbers) = line.split_once(": ").expect("Failure slicing string!");
    let (str_wn, str_sn) = numbers.split_once(" | ").expect("Failure slicing string!");

    let wn: Vec<u32> = str_wn
        .split_ascii_whitespace()
        .map(|s| s.parse::<u32>().unwrap())
        .collect();

    let sn: Vec<u32> = str_sn
        .split_ascii_whitespace()
        .map(|s| s.parse::<u32>().unwrap())
        .collect();

    (wn, sn)
}

fn read_inputs(path: &str) -> String {
    fs::read_to_string(&path).expect("Failure reading file!")
}

fn get_args() -> (option, filename) {
    let args: Vec<String> = env::args().collect();
    if args.len() == 3 {
        return (args[1].clone(), args[2].clone());
    }
    panic!("Too few args!");
}
