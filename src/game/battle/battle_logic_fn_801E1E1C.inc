    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r24, 0x70(r1)
    mr r24, r4
    mr r25, r5
    mr r30, r6
    mr r31, r7
    mr r26, r10
    mr r4, r3
    lha r27, 0x9a(r1)
    mr r29, r8
    clrlwi r5, r8, 16
    mr r28, r9
    clrlwi r6, r9, 16
    addi r3, r1, 0x48
    li r7, 0x1
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800BA9E4
    lfs f1, -21004(r2)
    addi r3, r1, 0x48
    li r4, 0x0
    li r5, 0x0
    fmr f2, f1
    li r6, 0x0
    fmr f3, f1
    li r7, 0x0
    li r8, 0x0
    bl fn_800BACA0
    addi r3, r1, 0x48
    li r4, 0x0
    bl fn_800BAFFC
    extsh r3, r29
    extsh r0, r28
    srawi r28, r3, 1
    mr r4, r24
    srawi r29, r0, 1
    addi r3, r1, 0x28
    clrlwi r5, r28, 16
    li r7, 0x1
    clrlwi r6, r29, 16
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800BA9E4
    lfs f1, -21004(r2)
    addi r3, r1, 0x28
    li r4, 0x0
    li r5, 0x0
    fmr f2, f1
    li r6, 0x0
    fmr f3, f1
    li r7, 0x0
    li r8, 0x0
    bl fn_800BACA0
    addi r3, r1, 0x28
    li r4, 0x1
    bl fn_800BAFFC
    mr r4, r25
    addi r3, r1, 0x8
    clrlwi r5, r28, 16
    clrlwi r6, r29, 16
    li r7, 0x1
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800BA9E4
    lfs f1, -21004(r2)
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0x0
    fmr f2, f1
    li r6, 0x0
    fmr f3, f1
    li r7, 0x0
    li r8, 0x0
    bl fn_800BACA0
    addi r3, r1, 0x8
    li r4, 0x2
    bl fn_800BAFFC
    li r3, 0x80
    li r4, 0x7
    li r5, 0x4
    bl fn_800B928C
    lis r4, 0xcc01
    extsh r0, r31
    sth r30, -0x8000(r4)
    add r6, r0, r27
    li r3, 0x0
    extsh r5, r30
    sth r31, -0x8000(r4)
    extsh r0, r26
    add r5, r5, r0
    sth r3, -0x8000(r4)
    li r0, 0x1
    sth r3, -0x8000(r4)
    sth r3, -0x8000(r4)
    sth r5, -0x8000(r4)
    sth r31, -0x8000(r4)
    sth r3, -0x8000(r4)
    sth r0, -0x8000(r4)
    sth r3, -0x8000(r4)
    sth r5, -0x8000(r4)
    sth r6, -0x8000(r4)
    sth r3, -0x8000(r4)
    sth r0, -0x8000(r4)
    sth r0, -0x8000(r4)
    sth r30, -0x8000(r4)
    sth r6, -0x8000(r4)
    sth r3, -0x8000(r4)
    sth r3, -0x8000(r4)
    sth r0, -0x8000(r4)
    lmw r24, 0x70(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
