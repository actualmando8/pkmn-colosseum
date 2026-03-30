    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r4
    mr r31, r5
    li r4, 0x0
    li r5, 0x4c
    bl fn_801FB1C0
    mr. r30, r3
    beq @L_801FBCFC
    mr r3, r29
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x73
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 24
    li r3, 0x0
    li r5, 0x11
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r0, r31, 24
    mr r31, r3
    bne @L_801FBCA0
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    b @L_801FBCFC
    @L_801FBCA0:
    cmplwi r0, 0x1
    bne @L_801FBCBC
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    bl fn_801DA9E8
    b @L_801FBCFC
    @L_801FBCBC:
    cmplwi r0, 0x2
    bne @L_801FBCE4
    @L_801FBCC4:
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    bl fn_801DA94C
    clrlwi. r0, r3, 24
    beq @L_801FBCFC
    bl fn_800F0308
    b @L_801FBCC4
    @L_801FBCE4:
    cmplwi r0, 0x3
    bne @L_801FBCFC
    mr r3, r30
    clrlwi r4, r31, 16
    li r5, 0x4
    bl fn_801DA8C4
    @L_801FBCFC:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
