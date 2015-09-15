#!/usr/bin/python
import argparse
#import matplotlib.pyplot as plt
#import networkx as nx
import time
from Bio import SeqIO
from multiprocessing import Manager
from multiprocessing import Process
from multiprocessing import Queue

def print_time(msg, start, stop):
    print '\t' + msg + ':\t' + str(stop - start)

def run_map(num_threads, size, target, args):
    workers = []
    for i in xrange(num_threads):
        i_start = i * size
        i_stop = min(i_start + size, len(args[0]))
        worker = Process(target=target, args=(i_start,) + (i_stop,) + args)
        workers.append(worker)
    for worker in workers:
        worker.start()
    for worker in workers:
        worker.join()

def load_reference(ref_file):
    with open(ref_file) as f:
        return str(SeqIO.parse(f, 'fasta').next().seq).upper()
    return ''

def load_reads(read_files, k):
    reads = []
    for read_file in read_files.split(','):
        with open(read_file) as f:
            for rec in SeqIO.parse(f, 'fastq'):
                read = str(rec.seq).upper()
                reads.append((rec.id,read))
    return reads

def map_index(i_start, i_stop, reads, k, idx):
    for i in xrange(i_start, i_stop):
        read = reads[i]
        for j in xrange(len(read[1]) - k):
            pos = j + k
            seed = read[1][j:pos]
            match = read[1]
            rec = ((read[0],pos),match)
            support = idx.get(seed)
            if support is None:
                idx[seed] = [rec]
            else:
                support.append(rec)

def map_variants(i_start, i_stop, ref, idx, k, q):
    #variants = [None for i in xrange(i_start - i_stop)]
    #variants = []
    for i in xrange(i_start, i_stop):
        pos = i + k
        seed = ref[i:pos]
        match = ref[pos]
        reads = idx.get(seed, [])
        support = [read for read in reads if match != read[1]]
        #variants.append(support)
    #print 'variants:', len(variants)
    #q.put(len(variants))
        if len(support) > 0:
            q += support
            #variants[pos] += support

def index_reads(num_threads, reads, k):
    idx = Manager().dict()
    args = (reads,k,idx,)
    run_map(num_threads, len(reads) / num_threads, map_index, args)
    return {key: val for key, val in idx.items()}

def call_variants(num_threads, ref, idx, k):
    q = Manager().list([[] for i in xrange(len(ref))])#Queue(100000000)
    args = (ref,idx,k,q,)
    run_map(num_threads, (len(ref) - k) / num_threads, map_variants, args)
    #variants = []
    #for i in xrange(num_threads):
    #    variants += [q.get()]
    #return variants
    return q

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-g', '--ref_file', required=True)
    parser.add_argument('-r', '--read_files', required=True)
    parser.add_argument('-k', '--k', default=30)
    parser.add_argument('-t', '--num_threads', default=1)
    args = parser.parse_args()

    k = int(args.k)
    num_threads = int(args.num_threads)

    start = time.time()
    ref = load_reference(args.ref_file)
    stop = time.time()
    print_time('load_reference', start, stop)

    start = time.time()
    reads = load_reads(args.read_files, k)
    stop = time.time()
    print_time('load_reads', start, stop)

    start = time.time()
    idx = index_reads(num_threads, reads, k)
    stop = time.time()
    print_time('index_reads', start, stop)
    print 'idx:', len(idx)

    start = time.time()
    variants = call_variants(num_threads, ref, idx, k)
    stop = time.time()
    print_time('call_variants', start, stop)
    print 'variants:', len(variants)

    #workers = []
    #for i in xrange(num_threads):
    #    i_start = i * size
    #    i_stop = min(i_start + size, len(ref) - k)
    #
    #    worker = Process(target=call_variants, args=args)
    #    workers.append(worker)
    #for worker in workers:
    #    worker.start()

    #stop = time.clock()
    #print_time('call_variants', start, stop)
    #print len(variants)

    #deps = {}
    #for key, val in variants.items():
    #    for read in val:
    #        pos = deps.get(read)
    #        if pos is None:
    #            deps[read] = [key]
    #        else:
    #            pos.append(key)

    #print '\t\treference_len: ', len(ref)
    #print '\t\tnum_read_parts:', len(reads)
    #print '\t\tnum_variants:  ', len(variants)
    #print '\t\tnum_reads:', len(deps)

    #G = nx.Graph()
    #for key, val in variants.items():
    #    G.add_node(key)
    #    for read in val:
    #        G.add_edges_from([(key,dep) for dep in deps[read] if dep != key])

    #nx.draw(G)
    #plt.show()

if __name__ == '__main__':
    main()
