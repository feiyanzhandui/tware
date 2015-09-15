download_start <- proc.time()
data_file = read.csv("") #read csv file with name "data.csv"
download_end <- proc.time()

process_start <- proc.time()

data_length <- length(data_file[,1])
num_features <- (length(data_file[1,]) - 1)

labels <- data_file[,(num_features + 1)]
features <- data_file[,1:num_features]

num_iters <- 20
alpha <- 1

theta <- matrix(0, 1, num_features)

for (c in 1:num_iters) {    # I think this works. I had to play with matrix operations
                            # to make it run faster, which makes the code a little harder to read
    # theta[j] <- theta[j] + alpha * sum((labels - features[,j] * theta[j]) * features[,j])
    theta <- theta + alpha * colSums((labels - sweep(features, theta, '*', MARGIN=2)) * features)
}

process_end <- proc.time()

print("Download time:")
print(download_end - download_start)
print("Runtime:")
print(process_end - process_start)
