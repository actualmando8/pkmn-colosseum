    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    beq @L_801FA8B8
    cmplwi r28, 0x0
    beq @L_801FA8B8
    cmplwi r27, 0x0
    beq @L_801FA840
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_8012A248
    mr r3, r27
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r27
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0x6
    bl fn_802077D4
    mr r3, r27
    li r4, 0x0
    li r5, 0x46
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0x2
    bl fn_80206C3C
    mr r3, r27
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801FB1C0
    bl fn_80206C94
    mr r3, r27
    li r4, 0x0
    li r5, 0x48
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    mr r3, r27
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r27
    li r4, 0x0
    li r5, 0x4a
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r27
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r27
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    mr r3, r27
    li r4, 0x0
    li r5, 0x4d
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801FA824
    bl fn_801F1460
    @L_801FA824:
    mr r3, r27
    li r4, 0x0
    li r5, 0x4e
    li r6, 0x0
    bl fn_801FB1C0
    li r4, 0xc
    bl fn_8020ECE0
    @L_801FA840:
    mr r3, r27
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801FA8B8
    mr r4, r28
    bl fn_8012AC64
    mr r3, r27
    clrlwi r7, r29, 16
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FAA58
    mr r3, r27
    mr r7, r30
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801FAA58
    cmplwi r31, 0x0
    beq @L_801FA8B8
    mr r3, r27
    mr r7, r31
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FAA58
    bl fn_801F198C
    @L_801FA8B8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
