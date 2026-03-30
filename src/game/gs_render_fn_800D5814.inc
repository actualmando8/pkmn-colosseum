    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r8, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lbz r0, 0x47e(r6)
    cmplwi r0, 0x0
    bne @L_800D5860
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800D5860
    mr r5, r3
    clrlwi r6, r4, 24
    clrlwi r7, r8, 24
    li r3, 0x1f
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D5890
    @L_800D5860:
    slwi r0, r3, 2
    lis r5, fn_800D72A4@ha
    addi r5, r5, fn_800D72A4@l
    slwi r7, r3, 4
    add r3, r6, r0
    stw r5, 0x500(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r7
    stb r4, 0x520(r3)
    lwz r0, lbl_8047AA80(r13)
    add r3, r0, r7
    stb r8, 0x521(r3)
    @L_800D5890:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
