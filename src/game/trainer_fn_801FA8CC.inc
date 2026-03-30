    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr. r28, r3
    beq @L_801FAA44
    clrlwi r31, r4, 16
    li r29, 0x0
    b @L_801FAA38
    @L_801FA8F0:
    clrlwi r0, r29, 16
    mulli r0, r0, 0x28e4
    add. r30, r28, r0
    beq @L_801FAA34
    mr r3, r30
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_8012A248
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r30
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0x6
    bl fn_802077D4
    mr r3, r30
    li r4, 0x0
    li r5, 0x46
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0x2
    bl fn_80206C3C
    mr r3, r30
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_80206C94
    mr r3, r30
    li r4, 0x0
    li r5, 0x48
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    mr r3, r30
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r30
    li r4, 0x0
    li r5, 0x4a
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r30
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r30
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r30
    li r4, 0x0
    li r5, 0x4d
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801FAA18
    bl fn_801F1460
    @L_801FAA18:
    mr r3, r30
    li r4, 0x0
    li r5, 0x4e
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0xc
    bl fn_8020ECE0
    @L_801FAA34:
    addi r29, r29, 0x1
    @L_801FAA38:
    clrlwi r0, r29, 16
    cmplw r0, r31
    blt @L_801FA8F0
    @L_801FAA44:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
