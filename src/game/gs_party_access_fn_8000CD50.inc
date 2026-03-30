    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    li r3, 0x0
    li r4, 0x3
    li r5, 0x0
    bl fn_8012A5B0
    mr r31, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000CE00
    li r3, 0x0
    li r4, 0x3
    li r5, 0x1
    bl fn_8012A5B0
    mr r30, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000CE00
    lfs f1, lbl_8047B6E8(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x1
    li r0, 0x2
    sth r3, 0x1c(r1)
    mr r3, r31
    mr r4, r30
    addi r6, r1, 0x1c
    sth r0, 0x1e(r1)
    addi r8, r1, 0x8
    li r5, 0x1
    li r7, 0x2
    bl fn_8026132C
    lfs f1, lbl_8047B6E8(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    @L_8000CE00:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
