    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r30, r4
    mr r29, r3
    mr r26, r5
    mr r24, r6
    mr r31, r7
    mr r23, r8
    li r4, 0x0
    bl fn_8020D8C8
    mr r3, r29
    li r4, 0x0
    bl fn_8020D8B8
    mr r3, r29
    li r4, 0x0
    bl fn_8020D8A8
    li r25, 0x0
    b @L_801F1230
    @L_801F121C:
    mr r3, r29
    mr r4, r25
    li r5, 0x0
    bl fn_8020D844
    addi r25, r25, 0x1
    @L_801F1230:
    clrlwi r0, r25, 16
    cmplwi r0, 0x4
    blt @L_801F121C
    mr r3, r29
    li r4, 0x0
    bl fn_8020D888
    mr r3, r29
    li r4, 0x0
    bl fn_8020D878
    mr r3, r29
    li r4, 0x0
    bl fn_8020D898
    mr r3, r29
    li r4, 0x0
    bl fn_8020D868
    mr r3, r29
    li r4, -0x1
    bl fn_8020D78C
    mr r3, r29
    mr r4, r24
    bl fn_8020D8C8
    mr r3, r29
    mr r4, r31
    bl fn_8020D8B8
    mr r3, r29
    mr r4, r23
    bl fn_8020D8A8
    mr r3, r29
    mr r4, r26
    bl fn_8020D898
    cmplwi r29, 0x0
    bne @L_801F12B8
    li r0, 0x0
    b @L_801F12EC
    @L_801F12B8:
    mr r3, r29
    bl fn_8020D950
    clrlwi. r0, r3, 16
    bne @L_801F12D0
    li r0, 0x0
    b @L_801F12EC
    @L_801F12D0:
    mr r3, r29
    bl fn_8020D920
    cmplwi r3, 0x0
    bne @L_801F12E8
    li r0, 0x0
    b @L_801F12EC
    @L_801F12E8:
    li r0, 0x1
    @L_801F12EC:
    clrlwi. r0, r0, 24
    bne @L_801F12FC
    li r24, 0x0
    b @L_801F1398
    @L_801F12FC:
    mr r3, r29
    bl fn_8020D950
    mr r27, r3
    mr r3, r29
    bl fn_8020D938
    mr r25, r3
    mr r3, r29
    bl fn_8020D920
    mr. r26, r3
    bne @L_801F132C
    li r24, 0x0
    b @L_801F137C
    @L_801F132C:
    li r23, 0x0
    clrlwi r28, r27, 16
    @L_801F1334:
    clrlslwi r0, r23, 16, 3
    add r24, r26, r0
    mr r3, r24
    bl fn_8020D82C
    clrlwi. r0, r3, 16
    mr r27, r3
    beq @L_801F1378
    mr r3, r24
    bl fn_8020D814
    clrlwi r0, r27, 16
    cmplw r28, r0
    bne @L_801F1370
    cmpw r25, r3
    bne @L_801F1370
    b @L_801F137C
    @L_801F1370:
    addi r23, r23, 0x1
    b @L_801F1334
    @L_801F1378:
    li r24, 0x0
    @L_801F137C:
    cmplwi r24, 0x0
    bne @L_801F138C
    li r24, 0x0
    b @L_801F1398
    @L_801F138C:
    mr r3, r29
    mr r4, r24
    bl fn_8020D8A8
    @L_801F1398:
    cmplwi r24, 0x0
    bne @L_801F1430
    mr r3, r29
    li r4, 0x0
    bl fn_8020D8C8
    mr r3, r29
    li r4, 0x0
    bl fn_8020D8B8
    mr r3, r29
    li r4, 0x0
    bl fn_8020D8A8
    li r25, 0x0
    b @L_801F13E0
    @L_801F13CC:
    mr r3, r29
    mr r4, r25
    li r5, 0x0
    bl fn_8020D844
    addi r25, r25, 0x1
    @L_801F13E0:
    clrlwi r0, r25, 16
    cmplwi r0, 0x4
    blt @L_801F13CC
    mr r3, r29
    li r4, 0x0
    bl fn_8020D888
    mr r3, r29
    li r4, 0x0
    bl fn_8020D878
    mr r3, r29
    li r4, 0x0
    bl fn_8020D898
    mr r3, r29
    li r4, 0x0
    bl fn_8020D868
    mr r3, r29
    li r4, -0x1
    bl fn_8020D78C
    li r3, 0x4
    b @L_801F144C
    @L_801F1430:
    mr r3, r29
    mr r4, r31
    bl fn_8020D878
    mr r3, r29
    mr r4, r30
    bl fn_8020D868
    li r3, 0x1
    @L_801F144C:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
