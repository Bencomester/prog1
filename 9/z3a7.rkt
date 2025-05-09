#lang racket

;; Define the node structure for our LZW binary tree
(struct node (bit left right) #:mutable)

;; Create the root node and initialize the current pointer
(define root (node #\/ #f #f))
(define current-node root)

;; Debug counters
(define total-bytes-read 0)
(define total-bits-processed 0)
(define byte-counts (make-hash))

;; Reset pointer to root
(define (reset-to-root!)
  (set! current-node root))

;; Insert a bit ('0' or '1') into the tree
(define (insert-bit bit)
  (set! total-bits-processed (+ total-bits-processed 1))
  (cond
    [(char=? bit #\0)
     ;; For '0', go left
     (if (not (node-left current-node))
         (begin
           ;; Create new node if it doesn't exist and reset to root
           (set-node-left! current-node (node #\0 #f #f))
           (reset-to-root!))
         ;; Otherwise, move to the existing node
         (set! current-node (node-left current-node)))]
    [(char=? bit #\1)
     ;; For '1', go right
     (if (not (node-right current-node))
         (begin
           ;; Create new node if it doesn't exist and reset to root
           (set-node-right! current-node (node #\1 #f #f))
           (reset-to-root!))
         ;; Otherwise, move to the existing node
         (set! current-node (node-right current-node)))]))

;; Read and process the input file - matching C++ implementation closely
(define (process-file path)
  (printf "Processing file: ~a~n" path)
  (define in (open-input-file path #:mode 'binary))
  
  ;; Skip until the first newline (like in the C++ version)
  (let loop ()
    (define b (read-byte in))
    (when (and (not (eof-object? b)) (not (= b #x0a))) ;; 0x0a is newline
      (loop)))
  
  (printf "Skipped header line~n")
  
  ;; Flag to track if we're currently in a comment
  (define in-comment #f)
  
  ;; Process the file byte by byte
  (let process-loop ()
    (define b (read-byte in))
    (if (eof-object? b)
        (void) ; Exit the loop when end of file is reached
        (begin
          (set! total-bytes-read (+ total-bytes-read 1))
          (hash-set! byte-counts b (+ (hash-ref byte-counts b 0) 1))
          
          ;; Handle comment markers and special characters (just like C++)
          (cond
            [(= b #x3e) ;; '>' character starts comment
             (set! in-comment #t)
             (process-loop)]
            [(= b #x0a) ;; Newline ends comment
             (set! in-comment #f)
             (process-loop)]
            [in-comment  ;; Skip comment lines
             (process-loop)]
            [(= b #x4e) ;; 'N' character - skip
             (process-loop)]
            [else
             ;; Process all 8 bits of the byte, most significant bit first
             (let ([byte-value b])
               (for ([i (in-range 8)])
                 ;; Extract the most significant bit using mask 0x80 (binary 10000000)
                 (define bit (if (= (bitwise-and byte-value #x80) 0) #\0 #\1))
                 ;; Shift left by 1 bit for next iteration
                 (set! byte-value (arithmetic-shift byte-value 1))
                 ;; Insert the bit into the tree
                 (insert-bit bit)))
             (process-loop)]))))
  
  (close-input-port in)
  (printf "Total bytes read: ~a~n" total-bytes-read)
  (printf "Total bits processed: ~a~n" total-bits-processed)
  (printf "Most common bytes:~n")
  (for ([pair (sort (hash->list byte-counts) > #:key cdr)])
    (when (> (cdr pair) 10)
      (printf "  ~a (#x~x): ~a times~n" 
              (if (or (< (car pair) 32) (> (car pair) 126))
                  "non-printable"
                  (integer->char (car pair)))
              (car pair) 
              (cdr pair))))
  )

;; Variables for statistics
(define depth-max 0)
(define depth-sum 0)
(define leaf-count 0)
(define depth-avg 0.0)
(define variance-sum 0.0)

;; Print the tree structure - FIXED to match C++ implementation
(define (print-tree node depth port)
  (when node
    (set! depth (+ depth 1))
    ;; Process right child first for proper display
    (print-tree (node-right node) depth port)
    
    ;; Print indentation - fixed to include depth + 1 dashes
    (for ([i (in-range depth)])
      (fprintf port "---"))
    
    ;; Print node with depth - 1 to match C++ implementation
    (fprintf port "~a(~a)~n" (node-bit node) (- depth 1))
    
    ;; Process left child
    (print-tree (node-left node) depth port)
    (set! depth (- depth 1))))

;; Calculate maximum depth
(define (calculate-max-depth node depth)
  (when node
    (when (> depth depth-max)
      (set! depth-max depth))
    (when (node-left node)
      (calculate-max-depth (node-left node) (+ depth 1)))
    (when (node-right node)
      (calculate-max-depth (node-right node) (+ depth 1)))))

;; Calculate average depth
(define (calculate-avg-depth node depth)
  (when node
    (when (node-left node)
      (calculate-avg-depth (node-left node) (+ depth 1)))
    (when (node-right node)
      (calculate-avg-depth (node-right node) (+ depth 1)))
    ;; Only count leaf nodes
    (when (and (not (node-left node)) (not (node-right node)))
      (set! depth-sum (+ depth-sum depth))
      (set! leaf-count (+ leaf-count 1)))))

;; Calculate variance
(define (calculate-variance node depth)
  (when node
    (when (node-left node)
      (calculate-variance (node-left node) (+ depth 1)))
    (when (node-right node)
      (calculate-variance (node-right node) (+ depth 1)))
    ;; Only count leaf nodes
    (when (and (not (node-left node)) (not (node-right node)))
      (set! variance-sum (+ variance-sum 
                            (sqr (- depth depth-avg)))))))

;; Analyze tree structure for debugging
(define (analyze-tree)
  (define (count-nodes node)
    (if node
        (+ 1 
           (count-nodes (node-left node))
           (count-nodes (node-right node)))
        0))
  
  (define (count-leaves node)
    (if node
        (if (and (not (node-left node)) (not (node-right node)))
            1
            (+ (count-leaves (node-left node))
               (count-leaves (node-right node))))
        0))
  
  (printf "Tree analysis:~n")
  (printf "  Total nodes: ~a~n" (count-nodes root))
  (printf "  Total leaves: ~a~n" (count-leaves root))
  (printf "  Max depth: ~a~n" depth-max))

;; Main function
(define (main in-file out-file)
  ;; Reset all statistics
  (set! depth-max 0)
  (set! depth-sum 0)
  (set! leaf-count 0)
  (set! depth-avg 0.0)
  (set! variance-sum 0.0)
  (set! total-bytes-read 0)
  (set! total-bits-processed 0)
  (set! byte-counts (make-hash))
  (set! root (node #\/ #f #f))
  (set! current-node root)
  
  ;; Process the file
  (process-file in-file)
  
  ;; Open output file
  (call-with-output-file out-file #:exists 'replace
    (lambda (out)
      ;; Print the tree
      (print-tree root 0 out)
      
      ;; Calculate statistics
      (calculate-max-depth root 0)
      (calculate-avg-depth root 0)
      
      ;; Calculate mean
      (when (> leaf-count 0)
        (set! depth-avg (/ depth-sum leaf-count))
        
        ;; Calculate variance
        (calculate-variance root 0)
        
        ;; Print statistics
        (fprintf out "depth = ~a~n" depth-max)
        (fprintf out "mean = ~a~n" depth-avg)
        (fprintf out "var = ~a~n" 
                 (if (> leaf-count 1)
                     (sqrt (/ variance-sum (- leaf-count 1)))
                     (sqrt variance-sum))))))
  
  ;; Print debug info
  (analyze-tree)
  
  ;; Signal successful completion
  (void))

;; Usage message
(define (usage)
  (displayln "Usage: racket lzwtree.rkt input.txt output.txt"))

;; Module main for command line use
(module+ main
  (define args (current-command-line-arguments))
  (if (= (vector-length args) 2)
      (main (vector-ref args 0) (vector-ref args 1))
      (usage)))

;; Export main function for interactive use
(provide main)