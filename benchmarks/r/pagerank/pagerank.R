download_start <- proc.time()
data_file = read.csv("") #read csv file with name "data.csv"
download_end <- proc.time()

data_file <- data.matrix(data_file)

process_start <- proc.time()

max_iters <- 20

num_pages <- length(data_file[,1])
num_links <- (length(data_file[1,]) - 1)

pages <- data_file[,(num_links + 1)]
links <- data_file[,1:num_links]

page_list <- new.env()  # get number of pages
num_pages <- 0
for (i in 1:data_length) {
    if (is.null(page_list[[as.character(pages[i])]])) {
        page_list[[as.character(pages[i])]] <- 1
        num_pages <- num_pages + 1
    }
}

link_vector <- matrix(0, nrow=num_pages, ncol=num_links)

for (i in 1:num_pages) {    # create matrix of what page leads to which pages
    for (j in 1:num_links) {
    link <- links[i,j]
        page <- pages[i]
        link_vector[page,link] <- link_vector[page,link] + 1
    }
}

link_vector <- link_vector / rowSums(link_vector)   # normalize matrix

page_vector <- matrix(1, ncol=num_pages, nrow=1)    # initialize page values to 1

for (c in 1:max_iters) {    # iterate through
    page_vector <- (page_vector %*% link_vector) * .85 + .15
}

process_end <- proc.time()

print("Download time:")
print(download_end - download_start)
print("Runtime:")
print(process_end - process_start)
