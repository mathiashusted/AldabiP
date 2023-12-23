mod alignment;

use alignment::alignment::Alignment;

fn main() {
    let mut instance = Alignment::new("MYMISSISAHIPPIE", "IMISSMISSISSIPPI");
    
    instance.compute(3, -4, -6);

    let mut a1: String = String::from("");
    let mut a2: String = String::from("");
    let mut gaps: String = String::from("");

    instance.get_alignment(&mut a1, &mut gaps, &mut a2);

    instance.print_matrix();

    println!("Score: {}", instance.get_score());
}
