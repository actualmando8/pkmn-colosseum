    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800D9C00
    li r3, 0x39
    li r4, 0xe
    creqv 6, 6, 6
    bl fn_800D4F98
    b @L_800D9C14
    @L_800D9C00:
    addi r3, r1, 0x8
    bl fn_800E0678
    addi r3, r1, 0x8
    li r4, 0x0
    bl fn_800BD2E0
    @L_800D9C14:
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
