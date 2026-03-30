    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800DBEF4
    lwz r0, 0x0(r4)
    mr r5, r3
    addi r6, r1, 0x8
    li r3, 0x50
    stw r0, 0x8(r1)
    li r4, 0x14
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBF0C
    @L_800DBEF4:
    lis r5, lbl_80400B28@ha
    slwi r3, r3, 2
    addi r0, r5, lbl_80400B28@l
    lwz r4, 0x0(r4)
    add r3, r0, r3
    stw r4, 0x1ec(r3)
    @L_800DBF0C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
