    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32735
    li r3, 0x0
    stw r0, 0x14(r1)
    addi r4, r4, 6776
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, -480
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    lis r3, -32712
    li r0, 0x0
    addi r3, r3, -24744
    addi r4, r13, -31400
    addis r3, r3, 0x1
    stb r0, 0x3(r4)
    stb r0, -20987(r13)
    stb r0, 0x4(r4)
    stb r0, 0x6002(r3)
    stb r0, 0x60a1(r3)
    bl fn_801DA7AC
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
