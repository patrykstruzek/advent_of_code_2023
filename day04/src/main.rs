use std::env;
use std::fs;

fn main() {
    let (option, path) = get_args();
    if option == "-p1" {
        let result = part1(&read_inputs(&path));
        println!("result: {}", { result });
    }
}

fn part1(inputs: &str) -> u32 {
    let mut result: u32 = 0;
    for line in inputs.lines() {
        result += parse_line(line.to_string());
    }
    result
}

fn parse_line(line: String) -> u32 {
    let (_card, numbers) = line.split_once(": ").expect("Failure slicing string!");
    let (wnumbers, ynumbers) = numbers.split_once(" | ").expect("Failure slicing string!");
    let wn: Vec<u32> = wnumbers
        .split_ascii_whitespace()
        .map(|s| s.parse::<u32>().unwrap())
        .collect();
    let yn: Vec<u32> = ynumbers
        .split_ascii_whitespace()
        .map(|s| s.parse::<u32>().unwrap())
        .collect();

    let mut counter: u32 = 0;
    for n in yn.iter() {
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
    return 0;
}

fn read_inputs(path: &str) -> String {
    fs::read_to_string(&path).expect("Failure reading file!")
}

fn get_args() -> (String, String) {
    let args: Vec<String> = env::args().collect();
    if args.len() == 3 {
        return (args[1].clone(), args[2].clone());
    }
    panic!("Too few args!");
}
