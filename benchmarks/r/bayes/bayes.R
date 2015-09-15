download_start <- proc.time()
data_file = read.csv("") #read csv file with name "data.csv"
download_end <- proc.time()

data_file <- data.matrix(data_file)

process_start <- proc.time()

data_length <- length(data_file[,1])
num_features <- (length(data_file[1,]) - 1)

labels <- data_file[,(num_features + 1)]
features <- data_file[,1:num_features]

label_list <- new.env() # get the number of labels
num_labels <- 0
for (i in 1:data_length) {
    if (is.null(label_list[[as.character(labels[i])]])) {
        label_list[[as.character(labels[i])]] <- 1
        num_labels <- num_labels + 1
    }
}

theta <- matrix(0, nrow=num_labels, ncol=num_features)

for (i in 1:data_length) {  # get totals
    theta[labels[i],] <- theta[labels[i],] + features[i,]
}
for (j in 1:num_features) { # normalize
    theta[,j] <- theta[,j] / sum(theta[,j])
}

process_end <- proc.time()

print("Download time:")
print(download_end - download_start)
print("Runtime:")
print(process_end - process_start)
