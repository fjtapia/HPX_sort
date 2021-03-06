<img alt="HPX icon"  src="doc/files/hpx.png">

<h1>HPX_sort</h1>
<h2> <a href="https://github.com/fjtapia/HPX_sort">https://github.com/fjtapia/HPX_sort</a></h2>


The **HPX sort**, had been designed for to be included in the [HPX Library](https://github.com/STEllAR-GROUP/hpx)

HPX is a general purpose C++ runtime system for parallel and distributed applications of any scale. The goal of HPX is to create a high quality, freely available, open source implementation of the ParalleX model for conventional systems, such as classic Linux based Beowulf clusters or multi-socket highly parallel SMP nodes.

This library provide the sort algorithms in a single thread and parallel versions for that Library.

The algorithms **use an comparison object**, in the same way than the sort algorithms of the standard library. If you don't define it, by default is std::less object, using internally the operator < for to do the comparison.

The algorithms are **exception safe**, it means,  the exceptions generated by the algorithms guarantee the integrity of the objects to sort , but not their relative order. If the exception is generated inside the objects (in the move or in the copy constructor.. ) the results can be unpredictable.

For to select the single thread and the parallel version, these algorithms use the execution policy, as described in the [Working Draft, Technical Specification for C++ Extensions for Parallelism](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4071.htm) . If you want a simplified version you can find in [cpp reference](http://en.cppreference.com/w/cpp/experimental/parallelism).

**This library is include only**. Don't need to link with any static or dynamic library. Don't have  dependence of any other files, variables or libraries or any other external libraries. For to use, only need a to include the files of the boost/sort/parallel folder, any more.

This table provide you a brief description of the sort algorithms of the library.

| Algorithm | Parallel | Stable | Additional memory | Best, average, and worst case |
| --- | --- | --- | --- | --- |
| sort | no | no | Log N  | NLogN, NLogN, NLogN |
| stable\_sort | no | yes| N / 2 | NLogN, NLogN, NLogN   |
| sort | yes | no | 1024\*NThreads | NLogN, NLogN, NLogN  |
| stable\_sort| yes | yes | N / 2 | NLogN, NLogN, NLogN   |
| sample\_sort | yes | yes | N  | NLogN, NLogN, NLogN  |

In this Parallel library, you can find **stable and not stable sort** algorithms, in a **single thread and parallel** version.

This version have a **new non stable parallel\_sort algorithm** *( internally named Block Indirect)*, created for processors connected with **shared memory**.

In the parallel sorting algorithms, we can find algorithms **fast with a small number of threads**, but with a great number of HW threads , show their lacks

- Intel Threading Building Blocks (TBB)
- Microsoft PPL Parallel Sort).

And others not so fast with small number of HW threads, and **fast with many threads**. These algorithms use an additional memory of the same size than the data.

 - GCC Parallel Sort (based on OpenMP)
 - Microsoft PPL Parallel Buffered Sort

This generate an **undesirable duality**. With a small number of threads use one algorithm, and with a big number use other. Due this, the SW designed for a small machine is inadequate for a big machine and vice versa. But the main problem, for the algorithms for a big number of HW threads is the memory used, usually of the same size than the data.

This **new parallel sort algorithm** permit **eliminate the duality**.  With  **many threads**, have similar performance than GCC Parallel Sort, and with  **a few threads** have similar performance than TBB, with the **additional advantage** of the **small memory consumption**.

The algorithm use as **auxiliary memory a 1024 elements** for each thread. The worst case for the algorithm is when have very big elements and many threads. With big elements (512 bytes), and 32 threads, The memory measured was:

| Algorithm | Memory used in MB |
| --- | --- |
| GCC Parallel Sort (OpenMP) | 1565 MB |
| Threading Building Blocks (TBB) | 783 MB |
| Block Indirect Sort | 814 MB |

*This **new parallel\_sort algorithm had been  created and implemented specifically for this library** by the author.*

*You can obtain more information in the Documantation Page of the Project  * [ index.html]( index.html)

*If you  are interested in a brief description of the algorithm, you can find in the next link* 
[block_indirect_sort_brief_en.pdf](block_indirect_sort_brief_en.pdf)

*If you  are interested in a detailed description of the algorithm, you can find in the next link* 
[block_indirect_sort_en.pdf](block_indirect_sort_en.pdf)



<h2>Installation </h2>
* This library is **include only**.  
 * Don't need to link with any static or dynamic library.
 * Don't have  dependence of any other libraries.
 * For to use, only need a to include the file hpx/parallel/sort/sort.hpp,.  


<h2>Author and Copyright</h2>
This library had been create for to be integrated in the [HPX](https://github.com/STEllAR-GROUP/hpx) library.

It's pending of revision, and due this can suffer some changes until the final version. You can find in  [https://github.com/fjtapia/HPX_sort](https://github.com/fjtapia/HPX_sort)

This algorithm had been ideate, designed and implemented beginning from zero. After read hundreds of articles and books, I didn't find any similar. If someone knows something about this or something similar, please, say me.

Anyway, the important is not the author, is provide a fast, robust, and easy to use algorithm to the community of programmers.

Copyright 2016  [Francisco Tapia *(fjtapia@gmail.com)* ](mail:fjtapia@gmail.com)
Distributed under the [Boost Software License, Version 1.0. ](http://www.boost.org/LICENSE_1_0.txt)  (See http://www.boost.org/LICENSE_1_0.txt)
