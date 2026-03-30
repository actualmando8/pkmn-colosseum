    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r5
    lwz r6, lbl_8047AA80(r13)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800DB948
    mr r5, r3
    mr r6, r4
    extsb r7, r30
    li r3, 0x5b
    li r4, 0x12
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DB970
    @L_800DB948:
    subi r0, r3, 0x1
    lis r3, lbl_80400B28@ha
    mulli r6, r0, 0x1c
    li r5, 0x18
    addi r0, r3, lbl_80400B28@l
    add r3, r0, r6
    addi r31, r3, 0x360
    mr r3, r31
    bl memcpy
    stb r30, 0x18(r31)
    @L_800DB970:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
