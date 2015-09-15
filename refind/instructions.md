Setup Instructions
------------------------
Dependencies

  * python 2
  * numpy
  * scikit-learn
  * pandas

Note that all dependencies (and mimic2 dataset) are installed on bsn #7

Starting Web Server
--------------------------
`cd tupleware/refind`

`python refind.py -p 8080`

This starts the web server and listens for JSON requests on port 8080.  To run the server continuously, use `screen`.


Loading Data
------------------
To load data from a client, issue a JSON query (you can use `curl` on the command line to test/debug) like so:

`curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"load","schema_file":"/data/demo_data/mimic2/mimic2_schema.json","data_file":"/data/demo_data/mimic2/mimic2.csv"}}' localhost:8080`

This loads the mimic2 dataset using the provided schema file (`/data/demo_data/mimic2/mimic2_schema.json`) and data file (`/data/demo_data/mimic2/mimic2.csv`) and stores the result in the cache in `uuid=0`.  

These files are located in the given locations on bsn #7.


Simple Operations
-------------------------
The following operations are currently supported.  Each operation takes one (or more) uuids as input (e.g., `source`, `left`, `right`) and saves the result in a new location (`uuid`).

  * **select** - select subset of input data satisfying `predicate` from `source` and put result in `uuid`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"select","source":"0", "uuid":"1","predicate":"age<18"}}' localhost:8080`

  * **project** - select a subset of columns `attributes` from `source` and put result in `uuid`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"project","source":"0","uuid":"2","attributes":["sex","height","weight"]}}' localhost:8080`

  * **union** - perform a union on the results stored in `sources` and put result in `uuid`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"union","sources":["1","2"], "uuid":"3"}}' localhost:8080`

  * **difference** - perform a set difference (`left` minus `right`) and put result in `uuid`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"difference","left":"1", "right":"2", "uuid":"3"}}' localhost:8080`

  * **intersect** - perform a set intersection of any number of `sources` and put result in `uuid`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"intersect","sources":["1","2"], "uuid":"3"}}' localhost:8080`

  * **merge** - concatenate *n* columns (`sources`) to create a new *n* dimensional dataset and put result in `uuid`.

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"merge","sources":["2","3"],"uuid":"4"}}' localhost:8080`


Advanced Operations
-----------------------------
The following more advanced operations are currently supported.  Each operation takes the uuid of one (or more) stored inputs and saves the result in a new location (`uuid`).

  * **correlate** - compute the pairwise correlation between all columns in `source` and put result in `uuid`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"correlate","source":"4","uuid":"5"}}' localhost:8080`

  * **classify** - build the specified `classifier` using the uuid of the *n*-dimensional `features` result and the uuid of the 1-dimensional `labels` result.  Supported classifiers include `logistic_regression`, `svm`, `naive_bayes`, and `random_forest`.

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"classify","params":{},"classifier":"logistic_regression","labels":"1", "features":"2","uuid":"3"}}' localhost:8080`

Requesting Results
--------------------------
To access data elements that have been computed, issue the following command.

`curl -H "Content-Type: application/json" -X POST -d {"type":"lookup","uuid":"4","page_size":"100","page_num":"0"}' localhost:8080`

This command returns the `page_num` page of size `page_size` of the result that is stored in `uuid=4`.

Example
-----------
The following example builds a logistic regression classifier on patients who are > 18 years old using `age, sex, height` as the features, and `metabolic` as the label.

  * load data into `uuid=0`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"load","schema_file":"/data/demo_data/mimic2/mimic2_schema.json","data_file":"/data/demo_data/mimic2/mimic2.csv", "uuid":"0"}}' localhost:8080`

  * perform selection on base data and store result in `uuid=1`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"select","source":"0", "uuid":"1","predicate":"age>18"}}' localhost:8080`

  * perform projection on `age, sex, height` to use as features for classifier and store result in `uuid=2`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"project","source":"1","uuid":"2","attributes":["age","sex","height"]}}' localhost:8080`

  * perform projection on `metabolic` to use as labels for classifier and store result in `uuid=3`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"project","source":"1","uuid":"3","attributes":["metabolic"]}}' localhost:8080`

  * build the logistic regression classifier and store summary results in `uuid=4`

  `curl -H "Content-Type: application/json" -X POST -d '{"type":"execute","task":{"type":"classify","params":{},"classifier":"logistic_regression","labels":"3","features":"2","uuid":"4"}}' localhost:8080`

  * request the summary results of the classifier (ROC curve, confusion matrix, etc)

  `curl -H "Content-Type: application/json" -X POST -d {"type":"lookup","uuid":"4","page_size":"100","page_num":"0"}' localhost:8080`
