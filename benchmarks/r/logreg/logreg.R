sigmoid <- function(x, a) {
    1 / (1 + exp(-a*(x)))
}

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

for (c in 1:num_iters) {    # Unfortunately, with the sigmoid function, I couldn't turn this
                            # into matrix operations, so it's really slow
    # theta[j] <- theta[j] + alpha * (labels[i] - sigmoid(features[i,j], theta[j])) * features[i,j]
    theta <- theta + alpha * colSums((labels - sweep(features, theta, sigmoid, MARGIN=2))*features)
}

process_end <- proc.time()

print("Download time:")
print(download_end - download_start)
print("Runtime:")
print(process_end - process_start)
