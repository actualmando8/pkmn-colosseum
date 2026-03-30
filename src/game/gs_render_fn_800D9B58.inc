    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    fmr f7, f1
    stw r0, 0x54(r1)
    fmr f0, f3
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D9B90
    li r3, 0x3a
    li r4, 0xe
    creqv 6, 6, 6
    bl fn_800D4F98
    b @L_800D9BC0
    @L_800D9B90:
    fmr f1, f2
    lfs f5, lbl_8047CA50(r2)
    fmr f2, f4
    lfs f6, lbl_8047CA54(r2)
    fmr f3, f7
    addi r3, r1, 0x8
    fmr f4, f0
    bl fn_800E0698
    addi r3, r1, 0x8
    li r4, 0x1
    bl fn_800BD2E0
    bl fn_800D834C
    @L_800D9BC0:
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
