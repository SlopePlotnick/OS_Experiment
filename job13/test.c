int inode_read(inode_t *this, off_t pos, void *mem, int size)
{
    if (pos >= this->size)
        return 0;
    if (pos + size > this->size)
        size = this->size - pos;

    void *p = mem;
    void *bnd = mem + size;
    while (p < bnd)
    {
        int bno = pos / BLOCK_SIZE;
        int offset = pos % BLOCK_SIZE;
        int chunk = min(bnd - p, BLOCK_SIZE - offset);

        bno = inode_map(this, bno);
        void *buff = disk_seek_block(bno);
        memcpy(p, buff + offset, chunk);

        pos += chunk;
        p += chunk;
    }

    return p - mem;
}