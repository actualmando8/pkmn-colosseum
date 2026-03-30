    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r3, -32698
    lis r4, -32728
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    addi r31, r3, 29688
    addi r30, r4, -27848
    addi r28, r31, 0x670
    addi r27, r31, 0x1c
    addi r26, r31, 0x21c
    addi r25, r31, 0x41c
@L_801D5494:
    bl fn_801DB088
    lwz r0, 0x0(r31)
    cmpwi r0, 0x1
    beq @L_801D54B4
    bge @L_801D56A4
    cmpwi r0, 0x0
    bge @L_801D54BC
    b @L_801D56A4
@L_801D54B4:
    bl fn_801D624C
    b @L_801D56A4
@L_801D54BC:
    lwz r4, 0x624(r31)
    lis r3, -32698
    addi r0, r3, 29688
    rlwinm r3, r4, 2, 0, 29
    add r3, r0, r3
    lwz r3, 0x61c(r3)
    cmplwi r3, 0x0
    beq @L_801D54FC
    li r4, 0x0
    li r5, 0x0
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801D54FC
    li r0, 0x0
    stw r0, 0x870(r31)
    b @L_801D56A4
@L_801D54FC:
    lwz r0, 0x870(r31)
    cmpwi r0, 0x0
    bgt @L_801D5694
    mr r3, r28
    bl fn_80167F28
    mr. r22, r3
    bne @L_801D5530
    mr r4, r28
    addi r3, r30, 0x6c
    crxor 6, 6, 6
    bl fn_800DD970
    li r29, 0x0
    b @L_801D5540
@L_801D5530:
    bl fn_80167E5C
    mr r29, r3
    mr r3, r22
    bl fn_80167E64
@L_801D5540:
    cmplwi r29, 0x0
    beq @L_801D5688
    addi r0, r29, 0x1f
    li r4, 0x20
    rlwinm r21, r0, 0, 0, 26
    mr r3, r21
    bl fn_800E2C04
    rlwinm. r0, r3, 0, 16, 31
    mr r22, r3
    beq @L_801D5688
    bl fn_800E27B0
    mr r23, r3
    mr r3, r28
    bl fn_80167F28
    mr. r24, r3
    bne @L_801D5594
    mr r4, r28
    addi r3, r30, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801D55D0
@L_801D5594:
    mr r4, r23
    mr r5, r21
    li r6, 0x0
    bl fn_80167ED0
    cmpwi r3, 0x0
    bge @L_801D55C8
    mr r4, r28
    addi r3, r30, 0xc8
    crxor 6, 6, 6
    bl fn_800DD970
    mr r3, r24
    bl fn_80167E64
    b @L_801D55D0
@L_801D55C8:
    mr r3, r24
    bl fn_80167E64
@L_801D55D0:
    lwz r0, 0x0(r23)
    cmpwi r0, 0x1
    bne @L_801D5678
    mr r3, r27
    addi r4, r23, 0x208
    li r5, 0x200
    bl memcpy
    mr r3, r26
    addi r4, r23, 0x408
    li r5, 0x200
    bl memcpy
    mr r3, r25
    addi r4, r23, 0x8
    li r5, 0x200
    bl memcpy
    lwz r3, 0x608(r23)
    li r0, 0x0
    stb r3, 0x87c(r31)
    stw r0, 0x4(r23)
    stw r0, 0x0(r23)
    bl fn_801D56B0
    mr r3, r28
    bl fn_80167F28
    mr. r24, r3
    beq @L_801D5668
    mr r4, r23
    mr r5, r29
    li r6, 0x0
    bl fn_80167E54
    cmpwi r3, 0x0
    bge @L_801D565C
    mr r4, r28
    addi r3, r30, 0xf8
    crxor 6, 6, 6
    bl fn_800DD970
@L_801D565C:
    mr r3, r24
    bl fn_80167E64
    b @L_801D5678
@L_801D5668:
    mr r4, r28
    addi r3, r30, 0x9c
    crxor 6, 6, 6
    bl fn_800DD970
@L_801D5678:
    mr r3, r22
    bl fn_800E24B0
    mr r3, r22
    bl fn_800E209C
@L_801D5688:
    li r0, 0x3c
    stw r0, 0x870(r31)
    b @L_801D56A4
@L_801D5694:
    bl fn_800D3088
    lwz r0, 0x870(r31)
    subf r0, r3, r0
    stw r0, 0x870(r31)
@L_801D56A4:
    bl fn_80177A64
    bl fn_800F0308
    b @L_801D5494
