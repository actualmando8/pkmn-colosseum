    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r5, lbl_8047AA80(r13)
    lwz r0, 0x0(r5)
    cmpwi r0, 0x1
    bne @L_800DBF54
    mr r5, r3
    mr r6, r4
    li r3, 0x4f
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DBF68
    @L_800DBF54:
    lis r5, lbl_80400B28@ha
    slwi r0, r3, 2
    addi r3, r5, lbl_80400B28@l
    add r3, r3, r0
    stw r4, 0x1ac(r3)
    @L_800DBF68:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
