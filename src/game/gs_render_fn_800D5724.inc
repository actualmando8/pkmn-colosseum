    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r7, lbl_8047AA80(r13)
    lbz r0, 0x47e(r7)
    cmplwi r0, 0x0
    bne @L_800D5768
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D5768
    mr r5, r3
    clrlwi r6, r4, 24
    li r3, 0x22
    li r4, 0x2
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D578C
    @L_800D5768:
    slwi r5, r3, 2
    lis r6, fn_800D724C@ha
    addi r6, r6, fn_800D724C@l
    slwi r0, r3, 4
    add r3, r7, r5
    stw r6, 0x500(r3)
    lwz r3, lbl_8047AA80(r13)
    add r3, r3, r0
    stb r4, 0x520(r3)
    @L_800D578C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
