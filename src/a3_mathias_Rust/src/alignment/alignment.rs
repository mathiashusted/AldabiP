/*
This is an implementation of the Needleman-Wunsch algorithm (aufgabe3) in Rust.
I did this mainly for the practice, and also to see the differences in performance
between a near identical implementation in C++ and Rust.
*/

/*
Tracebacks:
'0': None
'd': Diagonal
'v': Vertical
'h': Horizontal
*/

pub struct Alignment {
    seqv: String,
    seqh: String,
    f: Vec<Vec<i32>>,
    t: Vec<Vec<char>>,
    score: i32
}

impl Alignment {
    pub fn new(horizontal_sequence: &str, vertical_sequence: &str) -> Alignment {
        Alignment {
            seqh: String::from(horizontal_sequence),
            seqv: String::from(vertical_sequence),
            f: Vec::new(),
            t: Vec::new(),
            score: 0
        }
    }

    // Compute the alignment
    pub fn compute(&mut self, match_score: i32, mismatch_score: i32, gap: i32) {
        // Todo: Compute called?
        self.f.clear();
        self.t.clear();

        let width = self.seqh.len()+1;
        let height = self.seqv.len()+1;

        let mut current_score: i32;
        let mut max_score : i32;

        self.f.resize(width, vec![0; height as usize]);


        self.t.resize(width, vec!['0'; height as usize]);

        // Correct the first horizontal row to point towards (0,0)
        for i in 0..width {
            self.t[i][0] = 'h';
            self.f[i][0] = (i as i32)*gap;
        }
        for i in 0..height {
            self.t[0][i] = 'v';
            self.f[0][i] = (i as i32)*gap;
        }

        for row in 1..width {
            for line in 1..height {
                if self.seqh.chars().nth(row-1) == self.seqv.chars().nth(line-1) {
                    current_score = match_score;
                }
                else {
                    current_score = mismatch_score;
                }

                // Start looking for max

                max_score = self.f[row][line-1] + gap;
                self.t[row][line] = 'v';

                if self.f[row-1][line] + gap > max_score {
                    max_score = self.f[row-1][line] + gap;
                    self.t[row][line] = 'h';
                }
                if self.f[row-1][line-1] + current_score >= max_score {
                    max_score = self.f[row-1][line-1] + current_score;
                    self.t[row][line] = 'd';
                }

                self.f[row][line] = max_score;
            }
        }
        self.score = self.f[width-1][height-1];

    }

    // Just a simple function to return the score of the alignment made.
    pub fn get_score(&self) -> i32 {
        self.score
    }

    // Helper function in case we need to view the matrix
    pub fn print_matrix(&self) {
        for row in &self.f {
            for &element in row {
                print!("{} ",element);
            }
            print!("\n");
        }
    }

    // This function will put our results into 3 strings
    pub fn get_alignment(&self, a1: &mut String, gaps: &mut String, a2: &mut String) {
        a1.clear();
        gaps.clear();
        a2.clear();

        let mut i = self.seqh.len();
        let mut j = self.seqv.len();

        while !(i == 0 && j == 0) && (self.t[i][j] != '0') {
            if self.t[i][j] == 'd' {
                a1.push(self.seqv.chars().nth(j-1).unwrap());
                a2.push(self.seqh.chars().nth(i-1).unwrap());
                if self.seqv.chars().nth(j-1).unwrap() == self.seqh.chars().nth(i-1).unwrap() {
                    gaps.push('|');
                } else {
                    gaps.push(' ');
                }
                i -= 1;
                j -= 1;
            }
            else if self.t[i][j] == 'v' {
                a1.push(self.seqv.chars().nth(j-1).unwrap());
                a2.push('-');
                gaps.push(' ');
                j -= 1;
            }
            else {
                a1.push('-');
                a2.push(self.seqh.chars().nth(i-1).unwrap());
                gaps.push(' ');
                i -= 1;
            }
        }

        let mut reverser: Vec<char>;
        reverser = a1.chars().rev().collect();
        a1.clear();
        a1.extend(reverser);

        reverser = a2.chars().rev().collect();
        a2.clear();
        a2.extend(reverser);
        reverser = gaps.chars().rev().collect();
        gaps.clear();
        gaps.extend(reverser);

        println!("{}\n{}\n{}\n", a1, gaps, a2);


    }
}