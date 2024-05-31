/// Guessing Game
///
/// This small program prompts the user to input a number.
use std::cmp::Ordering::{Equal, Greater, Less};
use std::io;
use std::time::{SystemTime, UNIX_EPOCH};

fn randint() -> u32 {
    let big_num = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .subsec_nanos();

    return big_num % 100;
}

fn main() {
    println!("Guess the number!");

    let answer = randint();

    loop {
        let mut guess = String::new();

        println!("Input: ");
        io::stdin()
            .read_line(&mut guess)
            .expect("Failed to read line");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("Please guess a number!");
                continue;
            }
        };

        println!("You guessed: {guess}");

        // Tutorial really stretching to fit as many constructs as possible into a
        // simple program.
        match guess.cmp(&answer) {
            Less => println!("Too small"),
            Greater => println!("Too big"),
            Equal => {
                println!("You win!");
                break;
            }
        }
    }
}
