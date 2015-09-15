download_start <- proc.time()
data_file = read.csv("") #read csv file with name "data.csv"
download_end <- proc.time()

data_file <- data.matrix(data_file)

data_length <- length(data_file[,1])

process_start <- proc.time()

max_iters <- 20 # number of iterations
k <- 3
center <- array(rbind(c(25, 25), c(50, 50), c(75, 75)), dim=c(3, 2))    # centering points

for(j in 1:max_iters) {

    d <- array(0, dim=c(data_length, 2, 3)) # It's much, much faster when in a single array like this
    for (i in 1:k) {
        d[,,i] <- data_file[] - center[i,]
    }

    d <- d*d
    d <- sqrt(d[,1,] + d[,2,])

    c <- array(NaN, dim=c(data_length, 3, 2))

    for (i in 1:data_length) {
        c[i,which.min(d[i,]),] <- data_file[i,]
    }

    center <- colMeans(c, TRUE)
}

process_end <- proc.time()

print(center)

print("Download time:")
print(download_end - download_start)
print("Runtime:")
print(process_end - process_start)
