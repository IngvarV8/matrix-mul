use rand::Rng;
use std::fs::{File, OpenOptions};
use std::io::Write;
use std::path::Path;
use std::time::{Instant, SystemTime, UNIX_EPOCH};
use std::thread;

fn main() {
    let log_file: String = init_logger();
    let sizes = [500, 1000, 2000];

    for &size in &sizes {
        log_message(&log_file, format!("\nMultiplying matrices of size {}x{}...", size, size));

        let matrix1: Vec<Vec<i32>> = generate_matrix(size);
        let matrix2: Vec<Vec<i32>> = generate_matrix(size);

        let matrix1_f32: Vec<Vec<f32>> = generate_matrix_f32(size);
        let matrix2_f32: Vec<Vec<f32>> = generate_matrix_f32(size);

        //println!("Integer Matrix (first {}x{}):", size, size);
        //print_matrix(&matrix1, size);
        
        //println!("\nFloating-point Matrix (first {}x{}):", size, size);
        //print_matrix_f32(&matrix1_f32, size);

        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 1, &log_file);
        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 2, &log_file);
        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 4, &log_file);
        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 8, &log_file);
        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 12, &log_file);
        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 16, &log_file);
        multiply_matrices(matrix1.clone(), matrix2.clone(), size, 24, &log_file);
        

        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 1, &log_file);
        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 2, &log_file);
        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 4, &log_file);
        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 8, &log_file);
        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 12, &log_file);
        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 16, &log_file);
        multiply_matrices_f32(matrix1_f32.clone(), matrix2_f32.clone(), size, 24, &log_file);
    }
}

/// Creates a log file inside the `/logs/`
fn init_logger() -> String {
    let timestamp = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_secs();
    let log_filename = format!("logs/matrix_log_{}.txt", timestamp);

    if !Path::new("logs").exists() {
        std::fs::create_dir("logs").unwrap();
    }

    File::create(&log_filename).expect("Failed to create log file");
    
    log_filename
}

/// Appends a message to the log file
fn log_message(log_file: &str, message: String) {
    let mut file = OpenOptions::new()
        .append(true)
        .open(log_file)
        .expect("Failed to open log file");
    
    writeln!(file, "{}", message).expect("Failed to write to log file");
}

/// Generates a random matrix of given size
fn generate_matrix(size: usize) -> Vec<Vec<i32>> {
    let mut rng = rand::rng();
    (0..size)
        .map(|_| (0..size).map(|_| rng.random_range(0..100)).collect())
        .collect()
}

/// Generates a random matrix of given size with `f32` values
fn generate_matrix_f32(size: usize) -> Vec<Vec<f32>> {
    let mut rng = rand::rng(); // Initialize RNG
    (0..size)
        .map(|_| (0..size).map(|_| rng.random_range(0.0..100.0)).collect())
        .collect()
}

fn print_matrix(matrix: &Vec<Vec<i32>>, size: usize) {
    let n = size.min(10);  // Limit to 10x10 or the size of the matrix if smaller
    for row in matrix.iter().take(n) {  // Print only the first 'n' rows
        for &elem in row.iter().take(n) {  // Print only the first 'n' elements of each row
            print!("{:4} ", elem);
        }
        println!();
    }
}

fn print_matrix_f32(matrix: &Vec<Vec<f32>>, size: usize) {
    let n = size.min(10);  // Limit to 10x10 or the size of the matrix if smaller
    for row in matrix.iter().take(n) {  // Print only the first 'n' rows
        for &elem in row.iter().take(n) {  // Print only the first 'n' elements of each row
            print!("{:6.2} ", elem);  // Printing with two decimal places
        }
        println!();
    }
}

/// Multiplies matrices using multiple threads and logs performance
fn multiply_matrices(a: Vec<Vec<i32>>, b: Vec<Vec<i32>>, size: usize, threads_count: usize, log_file: &str) -> Vec<Vec<i32>> {
    let mut result = vec![vec![0; size]; size];

    if size < 1 || size < threads_count {
        log_message(log_file, "Error: size is either < 1 or less than thread count".to_string());
        return result;
    }

    let rows_per_thread = size / threads_count;
    let remainder = size % threads_count;

    let mut handles = Vec::new();

    let global_start_time = Instant::now();

    for i in 0..threads_count {
        let a_clone = a.clone();
        let b_clone = b.clone();

        let start_row = i * rows_per_thread;
        let end_row = if i == threads_count - 1 {
            start_row + rows_per_thread + remainder
        } else {
            start_row + rows_per_thread
        };

        let log_file_clone = log_file.to_string();
        
        let handle = thread::spawn(move || {
            let thread_start_time = Instant::now();

            let mut row_result = vec![vec![0; size]; end_row - start_row];
            for i in start_row..end_row {
                for j in 0..size {
                    let mut temp = 0;
                    for k in 0..size {
                        temp += a_clone[i][k] * b_clone[k][j];
                    }
                    row_result[i - start_row][j] = temp;
                }
            }

            let thread_duration = thread_start_time.elapsed();
            log_message(&log_file_clone, format!("Thread {} took {:?} to complete.", i, thread_duration));

            row_result
        });

        handles.push(handle);
    }
    
    let mut current_row = 0;
    for handle in handles {
        let row_result = handle.join().unwrap();
        for i in 0..row_result.len() {
            result[current_row + i] = row_result[i].clone();
        }
        current_row += row_result.len();
    }

    let global_duration = global_start_time.elapsed();
    log_message(
        log_file,
        format!(
            "Took {:?} to calculate {}x{} INTEGER32 matrix using {} threads.",
            global_duration, size, size, threads_count
        ),
    );

    result
}

/// Multiplies f32 matrices using multiple threads and logs performance
fn multiply_matrices_f32(a: Vec<Vec<f32>>, b: Vec<Vec<f32>>, size: usize, threads_count: usize, log_file: &str) -> Vec<Vec<f32>> {
    let mut result: Vec<Vec<f32>> = vec![vec![0.0; size]; size];

    if size < 1 || size < threads_count {
        log_message(log_file, "Error: size is either < 1 or less than thread count".to_string());
        return result;
    }

    let rows_per_thread = size / threads_count;
    let remainder = size % threads_count;

    let mut handles = Vec::new();

    let global_start_time = Instant::now();

    for i in 0..threads_count {
        let a_clone = a.clone();
        let b_clone = b.clone();

        let start_row = i * rows_per_thread;
        let end_row = if i == threads_count - 1 {
            start_row + rows_per_thread + remainder
        } else {
            start_row + rows_per_thread
        };

        let log_file_clone = log_file.to_string();
        
        let handle = thread::spawn(move || {
            let thread_start_time = Instant::now();

            let mut row_result: Vec<Vec<f32>> = vec![vec![0.0; size]; end_row - start_row];
            for i in start_row..end_row {
                for j in 0..size {
                    let mut temp: f32 = 0.0;
                    for k in 0..size {
                        temp += a_clone[i][k] * b_clone[k][j];
                    }
                    row_result[i - start_row][j] = temp;
                }
            }

            let thread_duration = thread_start_time.elapsed();
            log_message(&log_file_clone, format!("Thread {} took {:?} to complete.", i, thread_duration));

            row_result
        });

        handles.push(handle);
    }
    
    let mut current_row = 0;
    for handle in handles {
        let row_result = handle.join().unwrap();
        for i in 0..row_result.len() {
            result[current_row + i] = row_result[i].clone();
        }
        current_row += row_result.len();
    }

    let global_duration = global_start_time.elapsed();
    log_message(
        log_file,
        format!(
            "Took {:?} to calculate {}x{} FLOAT32 matrix using {} threads.",
            global_duration, size, size, threads_count
        ),
    );

    result
}