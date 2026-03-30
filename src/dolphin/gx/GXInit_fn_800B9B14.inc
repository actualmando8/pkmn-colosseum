    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lfs f0, -29592(r2)
    fdivs f1, f0, f1
    bl fn_800C46B0
    rlwinm r6, r3, 0, 23, 31
    lwz r4, -29632(r13)
    li r0, 0x61
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    oris r0, r6, 0x4e00
    li r5, 0x0
    stw r0, -0x8000(r3)
    subfic r3, r6, 0x100
    subic r0, r3, 0x1
    sth r5, 0x2(r4)
    subfe r0, r0, r3
    clrlslwi r0, r0, 24, 10
    lwz r3, 0x1ec(r4)
    cmplwi r6, 0x80
    addi r5, r6, 0x0
    rlwinm r3, r3, 0, 22, 20
    or r0, r3, r0
    stw r0, 0x1ec(r4)
    lwz r0, 0x1e4(r4)
    rlwinm r4, r0, 22, 22, 31
    rlwinm r0, r0, 30, 14, 23
    divwu r3, r0, r6
    addi r4, r4, 0x1
    addi r3, r3, 0x1
    ble @L_800B9BC0
    cmplwi r6, 0x100
    bge @L_800B9BC0
    b @L_800B9BA4
@L_800B9BA0:
    rlwinm r5, r5, 31, 1, 31
@L_800B9BA4:
    rlwinm. r0, r5, 0, 31, 31
    beq @L_800B9BA0
    divwu r0, r4, r5
    mullw r0, r0, r5
    subf. r0, r0, r4
    bne @L_800B9BC0
    addi r3, r3, 0x1
@L_800B9BC0:
    cmplwi r3, 0x400
    ble @L_800B9BCC
    li r3, 0x400
@L_800B9BCC:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
