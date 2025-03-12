use rand::Rng;
use std::time::Instant;
use std::thread;

fn main() {
    let sizes = [250, 500, 1000, 2000];
    
    for &size in &sizes {
        println!("\nMultiplying matrices of size {}x{}...", size, size);
        
        let matrix1 = generate_matrix(size);
        let matrix2 = generate_matrix(size);
        //let mut result = vec![vec![0; size]; size];
        multiply_matrices(matrix1, matrix2, size, 4);
        //multiply_matrices(matrix1, matrix2, size, 6);
    }
}

fn generate_matrix(size: usize) -> Vec<Vec<i32>> {
    let mut rng = rand::rng();
    (0..size).map(|_| (0..size).map(|_| rng.random_range(0..100)).collect()).collect()
}

fn multiply_matrices(a: Vec<Vec<i32>>, b: Vec<Vec<i32>>, size: usize, threads_count: usize) -> Vec<Vec<i32>> {
    let mut result = vec![vec![0; size]; size];

    if size < 1 || size < threads_count {
        print!("Error: size is either < 1 or less than thread count");
        return result;
    }

    let rows_per_thread = size / threads_count;
    let remainder = size % threads_count;

    let mut handles = Vec::new();

    let global_start_time = Instant::now();
    // Iterate over number of threads
    for i in 0..threads_count {
        let a_clone = a.clone();
        let b_clone = b.clone();

        let start_row = i * rows_per_thread;
        let end_row = if i == threads_count - 1 {
            start_row + rows_per_thread + remainder
        } else {
            start_row + rows_per_thread
        };

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
            println!("Thread {} took {:?} to complete.", i, thread_duration);

            row_result
        });

        handles.push(handle);
    }
    
    // Collect results from each thread and merge them into the result matrix
    let mut current_row = 0;
    for handle in handles {
        let row_result = handle.join().unwrap();
        for i in 0..row_result.len() {
            result[current_row + i] = row_result[i].clone();
        }
        current_row += row_result.len();
    }

    let global_duration = global_start_time.elapsed();
    println!("Took {:?} to calculate using {} threads.", global_duration, threads_count);

    result
}